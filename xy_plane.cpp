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
// MOVE SIMPLE (SIN FEEDBACK)
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
// PRINT ERROR (SOLO DIAGNÓSTICO)
// =============================================================
void printError() {
    delay(200);
    Serial1.print("JointAngle1: ");
    delay(10);
    Serial1.println(calculateJointAngle(targetShoulder, sensor1Offset, motor1Config.reduction), 1);

    delay(200);
    Serial1.print("JointAngle2: ");
    delay(10);
    Serial1.println(calculateJointAngle(targetElbow, sensor2Offset, motor2Config.reduction), 1);

    sensorSendAngle(Wire);  // Leer y enviar ángulo del segundo sensor
    sensorSendAngle(Wire1); // Leer y enviar ángulo del segundo sensor

    Serial1.println();
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
