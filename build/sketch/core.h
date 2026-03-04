#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\core.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : core.h
//  Autor      : Klaus Michalsky
//  Fecha      : Mar-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Declaración de funciones centrales del robot.
//      - Gestión de homing, flags globales y actualización de core.
//      - Separa la lógica de secuencia HOME-ALL de los homings individuales.
//      - Contiene variables globales accesibles desde otros módulos.
// =======================================================================

#pragma once
#include <Arduino.h>
#include "config.h"
#include "homingXY.h"
#include "homingZ.h"

// Máquina de estado para HOME_ALL
enum class HomeAllState
{
    IDLE,
    MOTOR1,
    MOTOR2,
    MOTOR3,
    DONE
};

extern bool dynamicAngle1;
extern bool dynamicAngle2;
extern HomingRunTimeXY homingMotor1;
extern HomingRunTimeXY homingMotor2;
extern HomingRunTimeZ homingMotor3;
extern HomeAllState homeAllState;
extern bool homeAllActive;

// Inicialización del core
void core_Init();

// Actualización periódica
void updateCore();

// Manejo de secuencias globales
void handleHomeAll();

void updateHoming();