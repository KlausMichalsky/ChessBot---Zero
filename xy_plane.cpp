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
// LECTURA REAL DE SENSORES (SIN COSAS RARAS)
// =============================================================
MotorAngles readXYAngles() {
    MotorAngles a;

    extern float sensor1Offset;
    extern float sensor2Offset;
    const float SHOULDER_OFFSET = -67.0f;
    // const float ELBOW_OFFSET = -89.0f;

    a.motorShoulder = sensorCorrectedAngle(Wire, sensor1Offset) - SHOULDER_OFFSET;
    a.motorElbow = sensorCorrectedAngle(Wire1, sensor2Offset);
    // RAW SENSOR + OFFSET
    // a.motorShoulder = sensorCorrectedAngle(Wire, sensorHomingOffset(Wire));
    // a.motorElbow = sensorCorrectedAngle(Wire1, sensorHomingOffset(Wire1));

    // normalización simple 0–360
    while (a.motorShoulder < 0)
        a.motorShoulder += 360;
    while (a.motorShoulder >= 360)
        a.motorShoulder -= 360;

    while (a.motorElbow < 0)
        a.motorElbow += 360;
    while (a.motorElbow >= 360)
        a.motorElbow -= 360;
    // if (a.motorShoulder < 0)
    //     a.motorShoulder += 360;
    // if (a.motorShoulder >= 360)
    //     a.motorShoulder -= 360;

    // if (a.motorElbow < 0)
    //     a.motorElbow += 360;
    // if (a.motorElbow >= 360)
    //     a.motorElbow -= 360;

    float fullStepsPerRev1 = motor1Config.microstepping * motor1Config.stepsPerRevolution * motor1Config.reduction;
    float fullStepsPerRev2 = motor2Config.microstepping * motor2Config.stepsPerRevolution * motor2Config.reduction;

    a.robotShoulder = (a.motorShoulder / 360.0f) * 360.0f;
    a.robotElbow = (a.motorElbow / 360.0f) * 360.0f;

    return a;
}

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
    MotorAngles a = readXYAngles();

    float errorShoulder = targetShoulder - a.robotShoulder;
    float errorElbow = targetElbow - a.robotElbow;

    // wrap simple [-180, 180]
    if (errorShoulder > 180)
        errorShoulder -= 360;
    if (errorShoulder < -180)
        errorShoulder += 360;

    if (errorElbow > 180)
        errorElbow -= 360;
    if (errorElbow < -180)
        errorElbow += 360;

    Serial1.println("===== ERROR REPORT =====");
    Serial1.print("Shoulder error: ");
    Serial1.println(errorShoulder, 3);

    Serial1.print("Elbow error: ");
    Serial1.println(errorElbow, 3);
    Serial1.println("========================");
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