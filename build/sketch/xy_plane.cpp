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
<<<<<<< HEAD
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
    motorsDisableXY();
}

// =============================================================
// UPDATE LOOP
=======
// UPDATE CINEMÁTICA
>>>>>>> KAYRON⚙️
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
    Serial1.println(sensor1);
    Serial1.print("Real Sensor Angle: ");
    Serial1.println(rawToDegrees(sensorReadRawAngle(Wire)));

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
    Serial1.println(sensor2);
    Serial1.print("Real Sensor Angle: ");
    Serial1.println(rawToDegrees(sensorReadRawAngle(Wire1)));

    Serial1.println();
}