// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : ChessBot---Zero.ino
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
// =======================================================================

#include <Arduino.h>
#include <Wire.h>

#include <AccelStepper.h>

#include "command.h"
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
    communicationInitUART();
    coreInit();
    motorsInit();
    sensorsInit();
    homingInitXY(homingMotor1);
    homingInitXY(homingMotor2);
    homingInitZ(homingMotor3);

    pinMode(IMAN, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
}

// LOOP
// -----------------------------------------------------------------------
void loop()
{
    if (CommandAvailable())
    {
        String cmd = receiveCommand();
        if (cmd.length() > 0) // 🔥 SOLO procesar si hay comando completo.
        {
            processCommand(cmd);
        }
    }
    coreUpdate();
}
