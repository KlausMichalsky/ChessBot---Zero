#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\xy_plane.h"
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

// HomingXY motor1Homing; → “esta es mi caja donde guardo datos de homing del motor 1”.
// MotorAngles readXYAngles(); → “dame una caja nueva con los ángulos actuales del motor y brazo”.
struct MotorAngles {
    float motorShoulder; // ángulo del sensor
    float motorElbow;    // ángulo del sensor
    float shoulder;      // ángulo del brazo
    float elbow;         // ángulo del brazo
};

extern bool xyMoving; // solo necesario para que otros archivos puedan leer el estado de movimiento

MotorAngles readXYAngles();
void moveToAngles(float shoulder, float elbow);
void updateXY();
bool xyIsMoving();