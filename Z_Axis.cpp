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

// Baja Z un número fijo de pasos definido en config.h
void zDown() {
    motor3.setMaxSpeed(200);
    motor3.setAcceleration(100);
    motor3.setCurrentPosition(0);
    motorEnableZ();
    zTarget = Z_STEPS_DOWN;
    motor3.moveTo(zTarget); // solo indica a donde ir
    zMoving = true;         // activa zMoving para que zStep() lo ejecute.
}

void zUp() {
    motor3.setMaxSpeed(200);
    motor3.setAcceleration(100);
    motorEnableZ();
    zTarget = 0;
    motor3.moveTo(zTarget);
    zMoving = true;
}

void zStep() {
    if (zMoving) {
        motor3.run(); // AccelStepper mueve y maneja aceleración
        if (motor3.distanceToGo() == 0) {
            zMoving = false;
        }
    }
}

void magnetON() {
}

void magnetOFF() {
}

// Z es bloqueante y se ejecuta desde command.cpp
void zPick() {
    zDown();
    while (zMoving)
        zStep();
    delay(Z_DELAY);
    magnetON();
    zUp();
    while (zMoving)
        zStep();
}

void zPlace() {
    zDown();
    while (zMoving)
        zStep();
    delay(Z_DELAY);
    magnetOFF();
    zUp();
    while (zMoving)
        zStep();
}