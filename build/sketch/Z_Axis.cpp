#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\Z_Axis.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : ZAxis.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definicion de las funciones para controlar el eje Z
// =======================================================================

#include <Arduino.h>

#include "Z_Axis.h"
#include "config.h"
#include "homing.h"
#include "motors.h"

static bool zMoving = false;
static long zTarget = 0;

void zStep() {
    if (zMoving) {
        motor3.run(); // AccelStepper mueve y maneja aceleración
        if (motor3.distanceToGo() == 0) {
            zMoving = false;
        }
    }
}

// Baja Z un número Z_STEPS_DOWN de pasos definido en config.h
void zMoveDown() {
    motor3.setMaxSpeed(6000);
    motor3.setAcceleration(15000);
    motor3.setCurrentPosition(0);
    zTarget = Z_STEPS_DOWN;
    motor3.moveTo(zTarget); // solo indica a donde ir
    zMoving = true;         // activa zMoving para que zStep() lo ejecute.
    while (zMoving)
        zStep();
}

void zMoveUp() {
    motor3.setMaxSpeed(6000);
    motor3.setAcceleration(15000);
    zTarget = 0;
    motor3.moveTo(zTarget);
    zMoving = true;
    while (zMoving)
        zStep();
}

void magnetON() {
    digitalWrite(MAGNET, HIGH);
}

void magnetOFF() {
    digitalWrite(MAGNET, LOW);
}

// Z es bloqueante y se ejecuta desde command.cpp
void zPick() {
    motorEnableZ();
    magnetOFF();
    zMoveDown();
    delay(Z_DELAY); // para mejorar controal al agarrar y soltar
    magnetON();
    delay(Z_DELAY);
    zMoveUp();
    motorDisableZ();
}

void zPlace() {
    motorEnableZ();
    zMoveDown();
    delay(Z_DELAY);
    magnetOFF();
    delay(Z_DELAY);
    zMoveUp();
    motorDisableZ();
}