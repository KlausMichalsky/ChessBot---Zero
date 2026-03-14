// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : z_axis.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definicion de las funciones
//      - Mover motor 1 (shoulder)
//      - Mover motor 2 (elbow)
//      - Recibir ángulos como input y usar feedback del AS5600
//      - Movimiento simultáneo no bloqueante
// =======================================================================

#include <Arduino.h>

#include "config.h"
#include "motors.h"
#include "sensors.h"
#include "xy_plane.h"

// Flag de movimiento
static bool xyMoving = false;

MotorAngles readXYAngles() {
    MotorAngles angles;

    // Angulo de motores
    angles.motorShoulder = sensorCorrectedAngle(Wire, sensorHomingOffset(Wire));
    angles.motorElbow = sensorCorrectedAngle(Wire1, sensorHomingOffset(Wire1));

    // Angulo del brazo
    angles.shoulder = angles.motorShoulder / motor1Config.reduction;
    angles.elbow = angles.motorElbow / motor2Config.reduction;

    return angles;
}

bool xyIsMoving() {
    return xyMoving;
}

void moveToAngles(float targetShoulder, float targetElbow) {
    if (xyMoving)
        return;

    // Convertir a pasos absolutos para AccelStepper
    long shoulderSteps = targetShoulder * motor1Config.reduction * motor1Config.stepsPerRevolution / 360.0;
    long elbowSteps = targetElbow * motor2Config.reduction * motor2Config.stepsPerRevolution / 360.0;

    motorsEnableXY();

    // Usar los pasos calculados, no los grados
    motor1.moveTo(shoulderSteps);
    motor2.moveTo(elbowSteps);

    xyMoving = true;
}

void updateXY() {
    if (!xyMoving)
        return;

    bool m1 = motor1.run(); // Devuelve true si todavía no llegó a la posición objetivo
    bool m2 = motor2.run();

    if (!m1 && !m2) { // ambos motores llegaron
        xyMoving = false;
    }
}