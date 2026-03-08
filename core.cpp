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

#include "command.h"
#include "core.h"
#include "homing.h"
#include "motors.h"
#include "sensors.h"

// FLAGS Y VARIABLES GLOBALES
// -----------------------------------------------------------------------
bool dynamicAngle1 = false;
bool dynamicAngle2 = false;

HomingXY motor1Homing;
HomingXY motor2Homing;
HomingZ motor3Homing;

HomeAllState homeAllState = HomeAllState::IDLE;

// -----------------------------------------------------------------------
void coreInit()
{
    homeAllState = HomeAllState::IDLE;
}

// -----------------------------------------------------------------------
void coreUpdate()
{
    if (homeAllState != HomeAllState::IDLE)
    {
        coreHomeAll();
    }
    else
    {
        coreHomeSingleMotor();
    }
}

// -----------------------------------------------------------------------
void coreHomeSingleMotor()
{
    // Homing Motor1
    static bool reported1 = false;
    if (homingXYisActive(motor1Homing))
        homingStepXY(motor1, motor1Config, motor1Homing, HALL_1);

    if (motor1Homing.state == HomingStateXY::OK && !reported1)
    {
        commandSendResponse("HOMING MOTOR1 OK"); // 🔹 evento “solo una vez”
        sensorSendAngle(Wire);
        reported1 = true;
    }
    else if (motor1Homing.state != HomingStateXY::OK)
        reported1 = false;

    // Homing Motor2
    static bool reported2 = false;
    if (homingXYisActive(motor2Homing))
        homingStepXY(motor2, motor2Config, motor2Homing, HALL_2);

    if (motor2Homing.state == HomingStateXY::OK && !reported2)
    {
        commandSendResponse("HOMING MOTOR2 OK");
        sensorSendAngle(Wire1);
        reported2 = true;
    }
    else if (motor2Homing.state != HomingStateXY::OK)
        reported2 = false;

    // Homing Motor3
    static bool reported3 = false;
    if (homingZisActive(motor3Homing))
        homingStepZ(motor3, motor3Config, motor3Homing, HALL_3);

    if (motor3Homing.state == HomingStateZ::OK && !reported3)
    {
        commandSendResponse("HOMING MOTOR3 OK");
        reported3 = true;
    }
    else if (motor3Homing.state != HomingStateZ::OK)
        reported3 = false;
}

// ⚠️ Solo para pruebas de lectura de angulo continuo
// -> El envio continuo bloquea movimiento de motores
void coreStreamAngles()
{
    if (dynamicAngle1)
        sensorStreamAngle(Wire, lastSentAngle_1, lastSendTime_1);
    if (dynamicAngle2)
        sensorStreamAngle(Wire1, lastSentAngle_2, lastSendTime_2);
}

// -----------------------------------------------------------------------
void coreHomeAll()
{
    // Ejecutar homings normalmente
    switch (homeAllState)
    {
    case HomeAllState::MOTOR1:
        if (motor1Homing.state == HomingStateXY::INACTIVE)
            homingStartXY(motor1, motor1Config, motor1Homing, HALL_1);

        homingStepXY(motor1, motor1Config, motor1Homing, HALL_1);

        if (motor1Homing.state == HomingStateXY::OK)
            homeAllState = HomeAllState::MOTOR2;
        break;

    case HomeAllState::MOTOR2:
        if (motor2Homing.state == HomingStateXY::INACTIVE)
            homingStartXY(motor2, motor2Config, motor2Homing, HALL_2);

        homingStepXY(motor2, motor2Config, motor2Homing, HALL_2);

        if (motor2Homing.state == HomingStateXY::OK)
            homeAllState = HomeAllState::MOTOR3;
        break;

    case HomeAllState::MOTOR3:
        if (motor3Homing.state == HomingStateZ::INACTIVE)
            homingStartZ(motor3, motor3Config, motor3Homing, HALL_3);

        homingStepZ(motor3, motor3Config, motor3Homing, HALL_3);

        if (motor3Homing.state == HomingStateZ::OK)
        {
            homeAllState = HomeAllState::IDLE; // ya terminó
        }
        break;

    default:
        break;
    }
}