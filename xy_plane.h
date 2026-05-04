// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : xy_axis.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Declaracion de funciones
// =======================================================================

#pragma once

#include <Arduino.h>

extern float sensor1Offset;
extern float sensor2Offset;

// HomingXY motor1Homing; → “esta es mi caja donde guardo datos de homing del motor 1”.
// MotorAngles readXYAngles(); → “dame una caja nueva con los ángulos actuales del motor y brazo”.
// float solo puede devolver un número decimal.
// Vos querés devolver varios ángulos a la vez: el ángulo del motor (motorShoulder, motorElbow) y el ángulo del brazo (shoulder, elbow).
// Por eso se usa un struct llamado MotorAngles que contiene todos esos campos:
struct MotorAngles {
    float motorShoulder; // ángulo del sensor
    float motorElbow;    // ángulo del sensor
    float robotShoulder; // ángulo del brazo
    float robotElbow;    // ángulo del brazo
};

MotorAngles readXYAngles();
void moveToAngles(float targetShoulder, float targetElbow);
void moveToAnglesFeedBack(float targetShoulderAngle, float targetElbowAngle);
void updateXY();
bool xyIsMoving();