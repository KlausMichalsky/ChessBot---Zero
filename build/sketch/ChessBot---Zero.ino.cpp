#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\ChessBot---Zero.ino"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : main_zero.ino
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN:
// =======================================================================

#include <Arduino.h>
#include <Wire.h>

#include <AccelStepper.h>
#include "commands.h"
#include "communication.h"
#include "config.h"
#include "core.h"
#include "homing.h"
#include "motors.h"
#include "sensors.h"
#include "utils.h"

// SETUP
// -----------------------------------------------------------------------
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

