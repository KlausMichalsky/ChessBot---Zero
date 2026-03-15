#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/xy_plane.cpp"
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
#include "utils.h"
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

void moveToAngles(float targetShoulderAngle, float targetElbowAngle) {
    if (xyMoving)
        return;

    motorsEnableXY();

    // Convertir a pasos absolutos para AccelStepper
    long targetShoulderSteps = angleToStep(targetShoulderAngle, MotorID::J1);
    long targetElbowSteps = angleToStep(targetElbowAngle, MotorID::J2);

    // Calcular la relación entre motor1 y 2
    long delta1 = abs(targetShoulderSteps - motor1.currentPosition());
    long delta2 = abs(targetElbowSteps - motor2.currentPosition());

    long maxDelta = max(delta1, delta2);

    float ratio1, ratio2;

    // Evitar división por cero
    if (maxDelta == 0) {
        ratio1 = ratio2 = 1.0;
    } else {
        ratio1 = (float)delta1 / maxDelta;
        ratio2 = (float)delta2 / maxDelta;
    }

    // Limitar ratio mínimo para que los motores no queden demasiado lentos
    ratio1 = max(ratio1, 0.2f); // mínimo 20% de velocidad
    ratio2 = max(ratio2, 0.2f);

    motor1.setMaxSpeed(motor1Config.baseSpeed * ratio1);
    motor2.setMaxSpeed(motor2Config.baseSpeed * ratio2);

    motor1.setAcceleration(motor1Config.acceleration * ratio1);
    motor2.setAcceleration(motor2Config.acceleration * ratio2);

    // Usar los pasos calculados, no los grados
    motor1.moveTo(targetShoulderSteps);
    motor2.moveTo(targetElbowSteps);

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