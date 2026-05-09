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

        float error1 =
            calculateError(
                targetShoulderAngle,
                Wire,
                motor1Config,
                sensor1Offset);

        float error2 =
            calculateError(
                targetElbowAngle,
                Wire1,
                motor2Config,
                sensor2Offset);

        printDebugMove(motor1Angle, motor2Angle);

        Serial1.print("Error1: ");
        Serial1.println(error1, 1);
        Serial1.print("Error2: ");
        Serial1.println(error2, 1);
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

    Serial1.print("Estimated Sensor Angle: ");
    Serial1.println(round1Decimal(sensor2), 1);
    Serial1.print("Real Sensor Angle: ");
    Serial1.println(rawToDegrees(sensorReadRawAngle(Wire1)), 1);

    Serial1.println();
}

void correctErrorOnce(
    float targetAngle,
    TwoWire &wire,
    const MotorConfig &config,
    float sensorOffset,
    MotorID id,
    AccelStepper &motor) {
    float error = calculateError(
        targetAngle,
        wire,
        config,
        sensorOffset);

    if (fabs(error) > 0.5f) {
        float correctedAngle =
            targetAngle - error;

        long correctedSteps =
            angleToStep(correctedAngle, id);

        motor.moveTo(correctedSteps);

        while (motor.distanceToGo() != 0) {
            motor.run();
        }
    }
}