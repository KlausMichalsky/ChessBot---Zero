// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : z_axis.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definicion de las funciones para controlar el eje Z
// =======================================================================

#include <Arduino.h>

#include "config.h"
#include "homing.h"
#include "motors.h"
#include "z_axis.h"

// VARIABLES LOCALES
// -----------------------------------------------------------------------
static bool zMoving = false;
static long zTarget = 0;

// MOVIMIENTO DEL EJE Z
// -----------------------------------------------------------------------
void zStep() {
    if (zMoving) {
        motor3.run(); // AccelStepper mueve y maneja aceleración
        if (motor3.distanceToGo() == 0) {
            zMoving = false;
        }
    }
}

// ASIGNACION DE DESTINO HACIA ABAJO (NO MUEVE TODAVIA)
// -----------------------------------------------------------------------
void zMoveDown() {
    motor3.setMaxSpeed(6000);
    motor3.setAcceleration(15000);
    motor3.setCurrentPosition(0);
    zTarget = Z_STEPS_DOWN; // Z_STEPS_DOWN pasos definido en config.h
    motor3.moveTo(zTarget);
    zMoving = true; // activa zMoving para que zStep() lo ejecute.
    while (zMoving)
        zStep();
}

// ASIGNACION DE DESTINO HACIA ARRIBA (NO MUEVE TODAVIA)
// -----------------------------------------------------------------------
void zMoveUp() {
    motor3.setMaxSpeed(6000);
    motor3.setAcceleration(15000);
    zTarget = 0;
    motor3.moveTo(zTarget);
    zMoving = true;
    while (zMoving)
        zStep();
}

// CONTROL DEL IMAN
// -----------------------------------------------------------------------
void magnetON() {
    digitalWrite(MAGNET, HIGH);
}

void magnetOFF() {
    digitalWrite(MAGNET, LOW);
}

// ZPick ZPlace SON BLOQUEANTES Y SE EJECUTAN DESDE COMMAND.CPP
// -----------------------------------------------------------------------
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