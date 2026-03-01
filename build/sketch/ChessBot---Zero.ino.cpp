#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : main_zero.ino
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Control principal del robot físico.
//      - Gestiona motores, sensores,
//        LEDs y la comunicación con el cerebro lógico del Pico.
//      - Inicializar y controlar motores y actuadores.
//      - Leer y procesar datos de sensores.
//      - Controlar los LEDs indicadores.
//      - Gestionar la comunicación con el Pico.
//      - Ejecutar los movimientos físicos del robot de ajedrez.
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <Bounce2.h>
#include "core.h"
#include "communication.h"
#include "commands.h"
#include "config.h"
#include "motors.h"
#include "sensors.h"
#include "homingXY.h"
#include "homingZ.h"
#include "calc.h"

Bounce debouncer; // Crea un objeto para el botón (solo en la fase de pruebas)

// SETUP
// -----------------------------------------------------------------------
#line 38 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
void setup();
#line 54 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
void loop();
#line 38 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
void setup()
{
    Serial.begin(115200);
    pinMode(IMAN, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
    UART_Init();
    sensors_Init();
    motors_Init();
    homingXY_Init(homingMotor1);
    homingXY_Init(homingMotor2);
    homingZ_Init(homingMotor3);
    core_Init();
}

// LOOP
// -----------------------------------------------------------------------
void loop()
{
    if (commandAvailable())
    {
        String cmd = receiveCommand();
        if (cmd.length() > 0) // 🔥 SOLO procesar si hay comando completo.
        {
            processCommand(cmd);
        }
    }
    updateCore();
}

