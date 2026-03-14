#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\xy_plane.cpp"
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
bool xyMoving = false;

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

void moveToAngles() {
    // Leer ángulos actuales del brazo
    MotorAngles a = readXYAngles();

    // Convertir a pasos absolutos para AccelStepper
    long shoulderSteps = a.shoulder * motor1Config.reduction * motor1Config.stepsPerRevolution / 360.0;
    long elbowSteps = a.elbow * motor2Config.reduction * motor2Config.stepsPerRevolution / 360.0;

    // Activar motores
    motorsEnableXY();

    // Mover motores
    motor1.moveTo(shoulderSteps);
    motor2.moveTo(elbowSteps);

    // Indicar que hay movimiento en curso
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