// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : commands.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones para la gestión de comandos
//        recibidos por UART
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"
#include "commands.h"
#include "homingXY.h"
#include "motorsXY.h"

extern HomingState homingMotor1;

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
        homingXY_Start(motor1, motor1Config, homingMotor1, HALL_1);
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
