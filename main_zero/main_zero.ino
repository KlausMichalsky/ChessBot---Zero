// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : main_zero.ino
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Control principal del robot físico. Gestiona motores, sensores,
//      LEDs y la comunicación con el cerebro lógico del Pico.
//  ▫️ RESPONSABILIDADES:
//      - Inicializar y controlar motores y actuadores.
//      - Leer y procesar datos de sensores.
//      - Controlar los LEDs indicadores.
//      - Gestionar la comunicación con el Pico.
//      - Ejecutar los movimientos físicos del robot de ajedrez.
// =======================================================================
//  ▫️ ESTADO:
//      ⚠️ EN DESARROLLO ⚠️
//      Funcionalidad básica implementada y en fase de pruebas funcionales.
//      Pruebas realizadas:
//      - Homing de motores con sensores HALL y AS5600 ✔️
//      - Control de motores con AccelStepper ✔️
//      Próximos pasos:
//      - Comunicación UART básica
//      - Lectura de sensores I2C AS5600
//      - Integración completa con el cerebro lógico (Pico)
//      - Implementación de movimientos XY y control de piezas
//      - Optimización de parámetros de homing y movimiento
//      - ...
// =======================================================================

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

// DEFINICION DE OBJETOS
// =======================================================================
HomingState homingMotor1;
HomingState homingMotor2;
Bounce debouncer; // Crea un objeto para el botón (solo en la fase de pruebas)

// SETUP
// =======================================================================
void setup()
{
    Serial.begin(115200);
    // Boton con debounce
    pinMode(BOTON, INPUT_PULLUP);
    debouncer.attach(BOTON);
    debouncer.interval(25); // 25 ms de debounce

    // LED indicador
    pinMode(LED, OUTPUT);

    UART_Init();
    motorsXY_Init();

    homingXY_Init(homingMotor1);
}

// LOOP
// =======================================================================
void loop()
{
    if (commandAvailable())
    {
        String cmd = receiveCommand();
        debug("Comando recibido: " + cmd); // solo imprime si DEBUG_UART = 1
        processCommand(cmd);
        // 🏠 arranca el homing si se recibe el comando "HOME"
    }
}
