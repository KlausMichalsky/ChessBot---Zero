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
static bool xyMoving = false;

static float targetShoulderAngle = 0;
static float targetElbowAngle = 0;

// =============================================================
bool xyIsMoving() {
    return xyMoving;
}

// =============================================================
// MOVIMIENTO OBJETIVO
// =============================================================
void moveToAngles(float shoulder, float elbow) {
    if (xyMoving)
        return;

    motorsEnableXY();

    targetShoulderAngle = shoulder;
    targetElbowAngle = elbow;

    long sSteps = angleToStep(shoulder, MotorID::J1);
    long eSteps = angleToStep(elbow, MotorID::J2);

    motor1.moveTo(sSteps);
    motor2.moveTo(eSteps);

    xyMoving = true;
}

// =============================================================
// UPDATE CINEMÁTICA
// =============================================================
void updateXY() {
    if (!xyMoving)
        return;

    bool m1 = motor1.run();
    bool m2 = motor2.run();

    if (!m1 && !m2) { // Cuando termina el movimiento
        xyMoving = false;
        printDebugMove(motor1Angle, motor2Angle);
    }
}

void printDebugMove(float motor1Angle, float motor2Angle) {
    // ----------------------------
    // SENSOR ANGLE (0–360)
    // ----------------------------
    float sensor1 = sensorCorrectedAngle(Wire, sensor1Offset);
    float sensor2 = sensorCorrectedAngle(Wire1, sensor2Offset);

    // ----------------------------
    // VUELTAS COMPLETAS
    // ----------------------------
    int turns1 = motor1Angle / 360.0f;
    int turns2 = motor2Angle / 360.0f;

    // ----------------------------
    // PRINT MOTOR 1
    // ----------------------------
    Serial1.println("---- MOTOR 1 ----");

    Serial1.print("Sensor angle: ");
    Serial1.println(sensor1, 1);

    Serial1.print("Motor angle (multi): ");
    Serial1.println(motor1Angle, 1);

    Serial1.print("Turns: ");
    Serial1.println(turns1);

    // ----------------------------
    // PRINT MOTOR 2
    // ----------------------------
    Serial1.println("---- MOTOR 2 ----");

    Serial1.print("Sensor angle: ");
    Serial1.println(sensor2, 1);

    Serial1.print("Motor angle (multi): ");
    Serial1.println(motor2Angle, 1);

    Serial1.print("Turns: ");
    Serial1.println(turns2);

    Serial1.println("--------------------");
    Serial1.println();
}