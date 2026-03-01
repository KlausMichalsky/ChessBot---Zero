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

// Inicialización del core
void core_Init();

// Actualización periódica
void updateCore();

// Manejo de secuencias globales
void handleHomeAll();

void updateHoming();