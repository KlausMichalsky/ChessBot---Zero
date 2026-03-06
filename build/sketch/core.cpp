#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\core.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : core.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Mar-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones centrales del robot.
//      - updateCore() gestiona homings y secuencias HOME-ALL.
//      - updateHoming() ejecuta homing de motores individuales
//        y lecturas de ángulos.
//      - handleHomeAll() ejecuta secuencia secuencial de homing
//        de los tres motores.
//      - Reinicia el estado de homings individuales cuando
//        finalizan para permitir nuevos comandos.
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
    homingXY_Step(motor1, motor1Config, homingMotor1, HALL_1);
    homingXY_Step(motor2, motor2Config, homingMotor2, HALL_2);
    homingZ_Step(motor3, motor3Config, homingMotor3, HALL_3);

    // Lectura continua de ángulos
    if (dynamicAngle1)
        sendDynamicAngle(Wire, lastSentAngle_1, lastSendTime_1);

    if (dynamicAngle2)
        sendDynamicAngle(Wire1, lastSentAngle_2, lastSendTime_2);

    // 🔹 Reiniciar homing individual después de completar
    if (homingMotor1.state == HomingStateXY::OK || homingMotor1.state == HomingStateXY::ERROR)
        homingMotor1.state = HomingStateXY::INACTIVE;

    if (homingMotor2.state == HomingStateXY::OK || homingMotor2.state == HomingStateXY::ERROR)
        homingMotor2.state = HomingStateXY::INACTIVE;

    if (homingMotor3.state == HomingStateZ::OK || homingMotor3.state == HomingStateZ::ERROR)
        homingMotor3.state = HomingStateZ::INACTIVE;
}

// -----------------------------------------------------------------------
void handleHomeAll()
{
    // 🔹 Detener cualquier homing activo previamente
    if (homingXY_IsActive(homingMotor1) && homeAllState != HomeAllState::MOTOR1)
        homingMotor1.state = HomingStateXY::INACTIVE;

    if (homingXY_IsActive(homingMotor2) && homeAllState != HomeAllState::MOTOR2)
        homingMotor2.state = HomingStateXY::INACTIVE;

    if (homingZ_IsActive(homingMotor3) && homeAllState != HomeAllState::MOTOR3)
        homingMotor3.state = HomingStateZ::INACTIVE;

    switch (homeAllState)
    {
    case HomeAllState::MOTOR1:
        if (homingMotor1.state == HomingStateXY::INACTIVE)
            homingXY_Start(motor1, motor1Config, homingMotor1, HALL_1);

        // 🔹 Ejecutar continuamente mientras no termine
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
            sendResponse("HOME ALL DONE");
            homeAllState = HomeAllState::IDLE;
        }
        break;

    default:
        break;
    }
}