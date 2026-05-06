// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

#include <Arduino.h>

#include "config.h"
#include "motors.h"
#include "sensors.h"
#include "utils.h"
#include "xy_plane.h"

// =============================================================
// FLAGS
// =============================================================
static bool xyMoving = false;
static float targetShoulder = 0;
static float targetElbow = 0;
static float errorShoulder = 0;
static float errorElbow = 0;

// =============================================================
// LECTURA REAL DE SENSORES
// =============================================================
// MotorAngles readXYAngles() {
// }

// =============================================================
// ESTADO
// =============================================================
bool xyIsMoving() {
    return xyMoving;
}

// =============================================================
// MOVER SIMPLE (SIN FEEDBACK)
// =============================================================
void moveToAngles(float shoulder, float elbow) {
    if (xyMoving)
        return;

    motorsEnableXY();

    targetShoulder = shoulder;
    targetElbow = elbow;

    long sSteps = angleToStep(shoulder, MotorID::J1);
    long eSteps = angleToStep(elbow, MotorID::J2);

    motor1.moveTo(sSteps);
    motor2.moveTo(eSteps);

    xyMoving = true;
}

// =============================================================
// CORREGIR ERROR (MOVIMIENTO DE FEEDBACK)
// =============================================================
void moveFeedback(float errorShoulder, float errorElbow) {
    motorsEnableXY();

    errorShoulder = angleError(targetShoulder, sensor1Offset, motor1Config.reduction, Wire);
    errorElbow = angleError(targetElbow, sensor2Offset, motor2Config.reduction, Wire1);

    Serial1.print("errorShoulder: ");
    Serial1.println(errorShoulder, 1);
    Serial1.print("errorElbow: ");
    Serial1.println(errorElbow, 1);

    long sTarget = motor1.currentPosition() + motor1Config.reduction * angleToStep(errorShoulder, MotorID::J1);
    long eTarget = motor2.currentPosition() + motor2Config.reduction * angleToStep(errorElbow, MotorID::J2);

    motor1.moveTo(sTarget);
    motor2.moveTo(eTarget);
}

// =============================================================
// PRINT ERROR (SOLO DIAGNÓSTICO)
// =============================================================
void printError() {
    Serial1.print("JointAngle1: ");
    Serial1.println(calculateJointAngle(targetShoulder, sensor1Offset, motor1Config.reduction), 1);

    Serial1.print("JointAngle2: ");
    Serial1.println(calculateJointAngle(targetElbow, sensor2Offset, motor2Config.reduction), 1);

    delay(100);
    Serial1.print("ActualAngle1: ");
    sensorSendAngle(Wire); // Leer y enviar ángulo del segundo sensor
    Serial1.print("ActualAngle2: ");
    sensorSendAngle(Wire1); // Leer y enviar ángulo del segundo sensor

    delay(100);
    Serial1.print("Error1: ");
    Serial1.println(angleError(targetShoulder, sensor1Offset, motor1Config.reduction, Wire), 1);
    delay(100);
    Serial1.print("Error2: ");
    Serial1.println(angleError(targetElbow, sensor2Offset, motor2Config.reduction, Wire1), 1);
    delay(500);
    Serial1.println();

    delay(2000);

    moveFeedback(errorShoulder, errorElbow);

    motorsDisableXY();
}

// =============================================================
// UPDATE LOOP
// =============================================================
void updateXY() {
    if (!xyMoving)
        return;

    bool m1 = motor1.run();
    bool m2 = motor2.run();

    if (!m1 && !m2) {
        xyMoving = false;

        delay(20); // estabilización mecánica mínima

        printError();
    }
}
