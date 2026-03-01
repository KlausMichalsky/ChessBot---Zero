#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/core.cpp"
#include <Arduino.h>
#include "core.h"
#include "motors.h"
#include "sensors.h"
#include "homingXY.h"
#include "homingZ.h"
#include "commands.h"

// FLAGS Y VARIABLES GLOBALES
// -----------------------------------------------------------------------
bool dynamicAngle1 = false; // Flag para lectura continua del ángulo AS5600
bool dynamicAngle2 = false; // Flag para lectura continua del ángulo AS5600
// Homing runtime
HomingRunTimeXY homingMotor1;
HomingRunTimeXY homingMotor2;
HomingRunTimeZ homingMotor3;

// Estado global
HomeAllState homeAllState = HomeAllState::IDLE;

void core_Init()
{
    homeAllState = HomeAllState::IDLE;
}

void updateCore()
{
    updateHoming();
    handleHomeAll();
}

void updateHoming()
{
    // Homing de motores (sin bloquear el loop)
    homingXY_Step(motor1, motor1Config, homingMotor1, HALL_1);
    homingXY_Step(motor2, motor2Config, homingMotor2, HALL_2);
    homingZ_Step(motor3, motor3Config, homingMotor3, HALL_3);

    // Lectura continua del ángulo 1 si el flag está activo
    if (dynamicAngle1)
    {
        sendDynamicAngle(Wire, lastSentAngle_1, lastSendTime_1);
        delay(20);
    }
    if (dynamicAngle2)
    {
        sendDynamicAngle(Wire1, lastSentAngle_2, lastSendTime_2);
        delay(20);
    }

    // Aqui se podrían agregar otras tareas periódicas,
    // como actualizar el estado de otros motores, leer sensores,
    // controlar LEDs, etc.
}

void handleHomeAll()
{
    switch (homeAllState)
    {
    case HomeAllState::MOTOR1:
        if (homingMotor1.state == HomingStateXY::INACTIVE)
            homingXY_Start(motor1, motor1Config, homingMotor1, HALL_1);
        if (homingMotor1.state == HomingStateXY::OK)
            homeAllState = HomeAllState::MOTOR2;
        break;

    case HomeAllState::MOTOR2:
        if (homingMotor2.state == HomingStateXY::INACTIVE)
            homingXY_Start(motor2, motor2Config, homingMotor2, HALL_2);
        if (homingMotor2.state == HomingStateXY::OK)
            homeAllState = HomeAllState::MOTOR3;
        break;

    case HomeAllState::MOTOR3:
        if (homingMotor3.state == HomingStateZ::INACTIVE)
            homingZ_Start(motor3, motor3Config, homingMotor3, HALL_3);
        if (homingMotor3.state == HomingStateZ::OK)
        {
            homeAllState = HomeAllState::DONE;
            sendResponse("HOME ALL DONE");
        }
        break;

    default:
        break;
    }
}