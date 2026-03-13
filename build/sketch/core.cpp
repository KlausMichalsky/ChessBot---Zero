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

// -----------------------------------------------------------------------
void coreInit() {
    homeAllState = HomeAllState::IDLE;
    homeSingleState = HomeSingleState::IDLE;
}

// -----------------------------------------------------------------------
void coreUpdate() {
    if (homeAllState != HomeAllState::IDLE) {
        coreHomeAll();
        return;
    }
    if (homeSingleState != HomeSingleState::IDLE) {
        coreHomeSingleMotor();
        return;
    }
}

void coreHomeSingleMotor() {
    // Motor 1
    if (homingXYisActive(motor1Homing)) {
        homingStepXY(motor1, motor1Config, motor1Homing, HALL_1);
    }

    if (motor1Homing.state == HomingStateXY::OK) {
        Serial1.print(motorStatus(MotorID::J1));
        homeSingleState = HomeSingleState::DONE;
        homingInitXY(motor1Homing);
        return;
    }

    // Motor 2
    if (homingXYisActive(motor2Homing)) {
        homingStepXY(motor2, motor2Config, motor2Homing, HALL_2);
    }

    if (motor2Homing.state == HomingStateXY::OK) {
        Serial1.print(motorStatus(MotorID::J2));
        homeSingleState = HomeSingleState::DONE;
        homingInitXY(motor2Homing);
        return;
    }

    // Motor 3
    if (homingZisActive(motor3Homing)) {
        homingStepZ(motor3, motor3Config, motor3Homing, HALL_3);
    }

    if (motor3Homing.state == HomingStateZ::OK) {
        Serial1.print(motorStatus(MotorID::Z));
        homeSingleState = HomeSingleState::DONE;
        homingInitZ(motor3Homing);
        return;
    }
}

// -----------------------------------------------------------------------
// void coreHomeSingleMotor() {
//     // Homing Motor1
//     static bool reported1 = false; // Flag para ejecutar el loop solo una vez
//     if (homingXYisActive(motor1Homing)) {
//         homingStepXY(motor1, motor1Config, motor1Homing, HALL_1);
//     }
//     if (motor1Homing.state == HomingStateXY::OK && !reported1) {
//         Serial1.print(motorStatus(MotorID::J1));
//         homeSingleState = HomeSingleState::DONE;
//         reported1 = true; // 👀 esto hace que el if se ejecute solo 1 vez en el loop
//         homingInitXY(motor1Homing);
//     } else if (motor1Homing.state != HomingStateXY::OK) {
//         reported1 = false;
//     }

//     // Homing Motor2
//     static bool reported2 = false;
//     if (homingXYisActive(motor2Homing)) {
//         homingStepXY(motor2, motor2Config, motor2Homing, HALL_2);
//     }
//     if (motor2Homing.state == HomingStateXY::OK && !reported2) {
//         Serial1.print(motorStatus(MotorID::J2));
//         homeSingleState = HomeSingleState::DONE;
//         reported2 = true;
//         homingInitXY(motor2Homing);
//     } else if (motor2Homing.state != HomingStateXY::OK) {
//         reported2 = false;
//     }

//     // Homing Motor3
//     static bool reported3 = false;
//     if (homingZisActive(motor3Homing)) {
//         homingStepZ(motor3, motor3Config, motor3Homing, HALL_3);
//     }
//     if (motor3Homing.state == HomingStateZ::OK && !reported3) {
//         Serial1.print(motorStatus(MotorID::Z));
//         homeSingleState = HomeSingleState::DONE;
//         reported3 = true;
//         homingInitZ(motor3Homing);
//     } else if (motor3Homing.state != HomingStateZ::OK) {
//         reported3 = false;
//     }
// }

// ⚠️ Solo para pruebas de lectura de angulo continuo
// -> El envio continuo bloquea movimiento de motores
void coreStreamAngles() {
    if (dynamicAngle1)
        sensorStreamAngle(Wire, lastSentAngle_1, lastSendTime_1);
    if (dynamicAngle2)
        sensorStreamAngle(Wire1, lastSentAngle_2, lastSendTime_2);
}

// -----------------------------------------------------------------------
void coreHomeAll() {
    // Ejecutar homings normalmente
    switch (homeAllState) {
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

            if (motor3Homing.state == HomingStateZ::OK) {
                homeAllState = HomeAllState::DONE;
                commandSendStatusReport();
                homingInitXY(motor1Homing);
                homingInitXY(motor2Homing);
                homingInitZ(motor3Homing);
            }
            break;

        case HomeAllState::DONE:
            homeAllState = HomeAllState::IDLE;
            break;

        default:
            break;
    }
}