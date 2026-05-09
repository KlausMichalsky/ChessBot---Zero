#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\xy_plane.cpp"
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
static bool correctionDone = false;

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

        float errorShoulder = calculateError(targetShoulderAngle, Wire, motor1Config, sensor1Offset);
        float errorElbow = calculateError(targetElbowAngle, Wire1, motor2Config, sensor2Offset);

        printDebugMove(motor1Angle, motor2Angle);

        Serial1.print("Error1: ");
        Serial1.println(errorShoulder, 1);
        Serial1.print("Error2: ");
        Serial1.println(errorElbow, 1);
        Serial1.println();

        delay(1000);

        // 🔥 corrección post-move (solo 1 vez)
        if (!correctionDone) {
            correctErrorOnce(errorShoulder, errorElbow);
            correctionDone = true; // 🔥 SOLO UNA VEZ
        }

        Serial1.print("post-move Real Sensor Angle1: ");
        Serial1.println(rawToDegrees(sensorReadRawAngle(Wire)), 1);
        Serial1.print("post-move Real Sensor Angle2: ");
        Serial1.println(rawToDegrees(sensorReadRawAngle(Wire1)), 1);
        Serial1.println();
    }
}

void printDebugMove(float motor1Angle, float motor2Angle) {
    // ----------------------------
    // PRINT MOTOR 1
    // ----------------------------
    Serial1.println();
    Serial1.println("-------- MOTOR 1 --------");

    float sensor1 = estimateSensorAngle(
        targetShoulderAngle,
        motor1Config.reduction,
        sensor1Offset,
        true,
        false);

    Serial1.print("HomingOffset: ");
    Serial1.println(sensor1Offset, 1);
    Serial1.print("Estimated Sensor Angle: ");
    Serial1.println(sensor1, 1);
    Serial1.print("Real Sensor Angle: ");
    Serial1.println(rawToDegrees(sensorReadRawAngle(Wire)), 1);

    // ----------------------------
    // PRINT MOTOR 2
    // ----------------------------
    Serial1.println("-------- MOTOR 2 --------");

    float sensor2 = estimateSensorAngle(
        targetElbowAngle,
        motor2Config.reduction,
        sensor2Offset,
        true,
        true);

    Serial1.print("HomingOffset: ");
    Serial1.println(sensor2Offset, 1);
    Serial1.print("Estimated Sensor Angle: ");
    Serial1.println(round1Decimal(sensor2), 1);
    Serial1.print("Real Sensor Angle: ");
    Serial1.println(rawToDegrees(sensorReadRawAngle(Wire1)), 1);

    Serial1.println();
}

void correctErrorOnce(float errorShoulder, float errorElbow) {
    // 🔥 SHOULDER
    if (fabs(errorShoulder) > 0.5f) {
        float correctedShoulder =
            targetShoulderAngle + errorShoulder;

        long sSteps =
            angleToStep(correctedShoulder, MotorID::J1);

        motor1.moveTo(sSteps);
    }

    // 🔥 ELBOW
    if (fabs(errorElbow) > 0.5f) {
        float correctedElbow =
            targetElbowAngle + errorElbow;

        long eSteps =
            angleToStep(correctedElbow, MotorID::J2);

        motor2.moveTo(eSteps);
    }

    xyMoving = true;
}