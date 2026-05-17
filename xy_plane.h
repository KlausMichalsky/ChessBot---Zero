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

void moveToAngles(float targetShoulder, float targetElbow);
void updateXY();
void updateMoveSequence();
bool xyIsMoving();
void printDebugMove(float motor1Angle, float motor2Angle);
void correctErrorOnce();
void startMoveSequence(float s1, float s2, float e1, float e2);