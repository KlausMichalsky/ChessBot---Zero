#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\motors.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : motors.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Declarar los motores utilizados por el sistema.
// =======================================================================

#pragma once

#include <Arduino.h>

#include <AccelStepper.h>

// DECLARACIÓN DE MOTORES (extern -> sin duplicar instancias)
// -----------------------------------------------------------------------
extern AccelStepper motor1;
extern AccelStepper motor2;
extern AccelStepper motor3;

// API PUBLICA FUNCIONES DE CONTROL DE MOTOR
// -----------------------------------------------------------------------
void motorsInit();
void motorsEnableXY();
void motorEnableZ();
void motorsDisableXY();
void motorDisableZ();
const char *motorName(MotorID id);
String motorStatus(MotorID id);
