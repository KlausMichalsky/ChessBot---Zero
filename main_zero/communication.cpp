// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : communication.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Implementación del módulo de comunicación del sistema.
//      Contiene la lógica para la inicialización de la UART,
//      la recepción y procesamiento de comandos, y el envío
//      de estados, errores y mensajes de depuración hacia el
//      cerebro lógico del robot.
//
//  ▫️ RESPONSABILIDADES:
//      - Configurar y arrancar la interfaz UART.
//      - Gestionar la recepción de datos de forma no bloqueante.
//      - Interpretar y procesar comandos entrantes.
//      - Enviar información estructurada del estado del sistema.
//      - Reportar errores y eventos relevantes.
//      - Emitir mensajes de depuración opcionales durante desarrollo.
// =======================================================================
//  ▫️ ESTADO:
//      ⚠️ EN DESARROLLO ⚠️
//      Implementación inicial del módulo de comunicación.
//      Pruebas realizadas:
//      - Inicialización básica de UART ✔️
//      - Envío de mensajes de estado y debug ✔️
//      Próximos pasos:
//      - Implementación completa del buffer de recepción no bloqueante.
//      - Validación y parseo robusto de comandos.
//      - Manejo de errores de comunicación.
//      - Integración total con el cerebro lógico (Pico / Pi).
//      - Optimización y limpieza para versión de producción.
// =======================================================================
#pragma once

#include <Arduino.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <Bounce2.h>
#include "communication.h"
#include "config.h"
#include "filtro.h"
#include "led.h"
#include "motorsXY.h"
#include "sensors.h"
#include "homingXY.h"

extern HomingState homingMotor1;

// CONFIGURACIÓN DE DEPURACIÓN UART
// =======================================================================
void debug(const String &msg)
{
#if DEBUG_UART
    Serial1.println(msg);
#endif
}

void UART_Init()
{
    // USB para debug (opcional)
    Serial.begin(115200);

    // UART hardware en pines 0 y 1
    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);
}

// COMPROBACIÓN DE COMANDOS DISPONIBLES
// =======================================================================
bool commandAvailable()
{
    return Serial1.available();
}

// LEER COMANDOS
// =======================================================================
String receiveCommand()
{
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    return cmd;
}

// PROCESAMIENTO DE COMANDOS
// =======================================================================
void processCommand(const String &cmd)
{
    if (cmd == "HOME")
    {
        Serial1.println("HOMING_STARTED");
    }
    else if (cmd == "STATUS")
    {
        Serial1.println("READY");
    }
}

// ENVIAR ESTADO
// =======================================================================
void sendStatus()
{
    // Enviar estado del robot al Pico
    String status = "STATUS:OK\n"; // Ejemplo de estado
    Serial1.print(status);
}

/*
void UART_Init()
{
#if DEBUG_UART
    Serial.begin(115200);
    unsigned long start = millis();
    while (!Serial && millis() - start < 1000)
    {
        // espera segura
    }
    Serial.println("Debug UART lista!");
#endif
    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);
}

// COMPROBACIÓN DE COMANDOS DISPONIBLES
// =======================================================================
//  ▫️ DESCRIPCIÓN
//      Verifica si hay datos disponibles para leer en la UART1.
//      👉 true = hay algo para leer
//      👉 false = no llegó nada
//      Serial1.available() → devuelve cuántos bytes hay en el buffer
//      > 0 → si hay al menos 1 byte, hay command
//  ▫️ RESPONSABILIDADES:
//      - Consultar el buffer de recepción de la UART1.
//      - Indicar si hay comandos listos para ser procesados.
bool commandAvailable()
{
    return Serial1.available() > 0;
}

// LEER COMANDOS
// ========================================================================
String receiveCommand()
{
    String command = Serial1.readStringUntil('\n');
    command.trim(); // quitar espacios o saltos de línea
    return command;
}

void processCommand(const String &command)
{
    // Aquí puedes implementar el procesamiento de comandos recibidos
    // desde el Pico u otros dispositivos.
    if (command == "HOME")
    {
        // Iniciar rutina de homing
        Serial1.println("HOMING_STARTED");
    }
    else if (command == "GET_ANGLE")
    {
        Serial1.println("READY"); // enviar al Pico que estamos listos
    }

    // Agrega más comandos según sea necesario
}

/*
    void processCommand(const String &command)
    {
        // Aquí puedes implementar el procesamiento de comandos recibidos
        // desde el Pico u otros dispositivos.
        if (command == "GET_ANGLE")
        {
            Serial1.println("READY"); // enviar al Pico que estamos listos
        }

        // Agrega más comandos según sea necesario
    }
    /*
        void leerAngulos()
        {
            // Leer ángulos de los motores y almacenarlos en variables globales
            // angulo_motor1 = obtenerAnguloMotor1();
            // angulo_motor2 = obtenerAnguloMotor2();
        }

        float recibirAngulos_UART()
        {
            Serial1.println("READY");   // Indicar al Pico que estamos listos para recibir
            while (Serial1.available()) // Mientras haya datos para leer
            {
                String linea = Serial1.readStringUntil('\n');

                // Buscar los caracteres y devolver el indice
                int guion = linea.indexOf('_');
                int dospuntos = linea.indexOf(':');

                if (guion != -1 && dospuntos != -1) // validar formato
                // indexOf → devuelve posición ≥ 0 si encuentra el carácter, -1 si no.
                // -1 es simplemente un valor especial que indica “no encontrado”.
                {
                    int motor_id = linea.substring(guion + 1, dospuntos).toInt();
                    float angulo = linea.substring(dospuntos + 2).toFloat(); // +2 para ": "

                    // Llamar función que mueva el motor
                    // moveMotor(motor_id, angulo);
                    return angulo;
                }
            }
            return -1; // Valor por defecto si no se recibe nada o formato incorrecto
        }

        void mandarAngulos(float angulo)
        {
            // Reenviar el angulo recibido al Pico para confirmación
            Serial1.print("ANGLE: ");
            Serial1.println(angulo);
        }

        void mandarStatus()
        {
            // Enviar estado del robot al Pico
            String status = "STATUS:OK\n"; // Ejemplo de estado
            Serial1.print(status);
        }
        */
