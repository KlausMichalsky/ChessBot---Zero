// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : core.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Mar-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones centrales del robot.
//      - Sincronización
// =======================================================================

#include <Arduino.h>
#include "core.h"
#include "motors.h"
#include "sensors.h"
#include "homingXY.h"
#include "homingZ.h"
#include "commands.h"

// FLAGS Y VARIABLES GLOBALES
bool dynamicAngle1 = false;
bool dynamicAngle2 = false;

// Homing runtime
HomingRunTimeXY homingMotor1;
HomingRunTimeXY homingMotor2;
HomingRunTimeZ homingMotor3;

// Estado global HOME-ALL
HomeAllState homeAllState = HomeAllState::IDLE;

// -----------------------------------------------------------------------
void core_Init()
{
    homeAllState = HomeAllState::IDLE;
}

// -----------------------------------------------------------------------
void updateCore()
{
    // 🔹 Si estamos en secuencia HOME-ALL, solo ejecutar handleHomeAll()
    if (homeAllState != HomeAllState::IDLE)
    {
        handleHomeAll();
    }
    else
    {
        // 🔹 Si no, actualizar homings individuales
        updateHoming();
    }
}

// -----------------------------------------------------------------------
void updateHoming()
{
    // Homing Motor1
    static bool reported1 = false;
    if (homingXY_IsActive(homingMotor1))
        homingXY_Step(motor1, motor1Config, homingMotor1, HALL_1);

    if (homingMotor1.state == HomingStateXY::OK && !reported1)
    {
        sendResponse("HOMING MOTOR1 OK"); // 🔹 evento “solo una vez”
        reported1 = true;
    }
    else if (homingMotor1.state != HomingStateXY::OK)
        reported1 = false;

    // Homing Motor2
    static bool reported2 = false;
    if (homingXY_IsActive(homingMotor2))
        homingXY_Step(motor2, motor2Config, homingMotor2, HALL_2);

    if (homingMotor2.state == HomingStateXY::OK && !reported2)
    {
        sendResponse("HOMING MOTOR2 OK");
        reported2 = true;
    }
    else if (homingMotor2.state != HomingStateXY::OK)
        reported2 = false;

    // Homing Motor3
    static bool reported3 = false;
    if (homingZ_IsActive(homingMotor3))
        homingZ_Step(motor3, motor3Config, homingMotor3, HALL_3);

    if (homingMotor3.state == HomingStateZ::OK && !reported3)
    {
        sendResponse("HOMING MOTOR3 OK");
        reported3 = true;
    }
    else if (homingMotor3.state != HomingStateZ::OK)
        reported3 = false;

    // Lectura ángulos dinámicos
    if (dynamicAngle1)
        sendDynamicAngle(Wire, lastSentAngle_1, lastSendTime_1);
    if (dynamicAngle2)
        sendDynamicAngle(Wire1, lastSentAngle_2, lastSendTime_2);
}

// -----------------------------------------------------------------------
void handleHomeAll()
{
    // Ejecutar homings normalmente
    switch (homeAllState)
    {
    case HomeAllState::MOTOR1:
        if (homingMotor1.state == HomingStateXY::INACTIVE)
            homingXY_Start(motor1, motor1Config, homingMotor1, HALL_1);

        homingXY_Step(motor1, motor1Config, homingMotor1, HALL_1);

        if (homingMotor1.state == HomingStateXY::OK)
            homeAllState = HomeAllState::MOTOR2;
        break;

    case HomeAllState::MOTOR2:
        if (homingMotor2.state == HomingStateXY::INACTIVE)
            homingXY_Start(motor2, motor2Config, homingMotor2, HALL_2);

        homingXY_Step(motor2, motor2Config, homingMotor2, HALL_2);

        if (homingMotor2.state == HomingStateXY::OK)
            homeAllState = HomeAllState::MOTOR3;
        break;

    case HomeAllState::MOTOR3:
        if (homingMotor3.state == HomingStateZ::INACTIVE)
            homingZ_Start(motor3, motor3Config, homingMotor3, HALL_3);

        homingZ_Step(motor3, motor3Config, homingMotor3, HALL_3);

        if (homingMotor3.state == HomingStateZ::OK)
        {
            homeAllState = HomeAllState::IDLE; // ya terminó
        }
        break;

    default:
        break;
    }
}