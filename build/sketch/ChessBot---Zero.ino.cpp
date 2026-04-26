#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
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
#line 27 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
void setup();
#line 43 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
void loop();
#line 27 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/ChessBot---Zero.ino"
void setup() {
    communicationInit();
    coreInit();
    homingInitXY(motor1Homing);
    homingInitXY(motor2Homing);
    homingInitZ(motor3Homing);
    motorsInit();
    sensorsInit();

    pinMode(MAGNET, OUTPUT);
    digitalWrite(MAGNET, LOW);
    pinMode(LED, OUTPUT);
}

// LOOP
// -----------------------------------------------------------------------
void loop() {
    if (commandAvailable()) {
        String cmd = readCommand();
        if (cmd.length() > 0) // 🔥 SOLO procesar si hay comando completo.
        {
            processCommand(cmd);
        }
    }
    coreUpdate();
}

