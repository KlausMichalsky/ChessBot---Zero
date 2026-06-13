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
#include "z_axis.h"

// VARIABLES LOCALES
// -----------------------------------------------------------------------
static float targetShoulderAngle = 0;
static float targetElbowAngle = 0;
static unsigned long settleStart = 0;

// DEFINICION DE MAQUINA DE ESTADOS PARA MOVIMIENTO XY-PLANE
// -----------------------------------------------------------------------
MovingStateXY movingStateXY = MovingStateXY::IDLE;
MoveSequenceState moveSeqState = MoveSequenceState::IDLE;

float startT1 = 0;
float startT2 = 0;
float endT1 = 0;
float endT2 = 0;

extern float currentShoulderAngle;
extern float currentElbowAngle;

// STATUS
// -----------------------------------------------------------------------
bool xyIsMoving() {
    return movingStateXY != MovingStateXY::IDLE;
}

// ASIGNACION DE DESTINO EN GRADOS (NO MUEVE TODAVIA)
// -----------------------------------------------------------------------
void moveToAngles(float shoulder, float elbow) {
    if (movingStateXY != MovingStateXY::IDLE)
        return;

    motorsEnableXY();

    // 🔥 1. CORRECCIÓN DE CONTINUIDAD (AQUÍ ES DONDE VA)
    float correctedShoulder = shortestAngle(shoulder, currentShoulderAngle);
    float correctedElbow = shortestAngle(elbow, currentElbowAngle);

    // 🔥 2. guardar targets corregidos
    targetShoulderAngle = correctedShoulder;
    targetElbowAngle = correctedElbow;

    // 🔥 3. convertir a pasos ya con valores suaves
    long sSteps = angleToStep(correctedShoulder, MotorID::J1);
    long eSteps = angleToStep(correctedElbow, MotorID::J2);

    motor1.moveTo(sSteps);
    motor2.moveTo(eSteps);

    movingStateXY = MovingStateXY::MOVING_TO_TARGET;
}

// ASIGNACION DEL DESTINO DEL ERROR EN GRADOS
// (UNA SOLA EJECUCIÓN POR CICLO, NO MUEVE TODAVIA)
// -----------------------------------------------------------------------
void correctErrorOnce() {
    motorsEnableXY();

    float errorShoulder =
        calculateError(targetShoulderAngle,
                       Wire,
                       motor1Config,
                       sensor1Offset);

    float errorElbow =
        calculateError(targetElbowAngle,
                       Wire1,
                       motor2Config,
                       sensor2Offset);

    bool needsCorrection = false;

    if (fabs(errorShoulder) > 0.5f) {
        float correctedShoulder =
            targetShoulderAngle +
            (motor1Config.motorDirection *
             errorShoulder / motor1Config.reduction);

        motor1.moveTo(
            angleToStep(correctedShoulder, MotorID::J1));

        needsCorrection = true;
    }

    if (fabs(errorElbow) > 0.5f) {
        float correctedElbow =
            targetElbowAngle +
            (motor2Config.motorDirection *
             errorElbow / motor2Config.reduction);

        motor2.moveTo(
            angleToStep(correctedElbow, MotorID::J2));

        needsCorrection = true;
    }

    if (needsCorrection) {
        movingStateXY = MovingStateXY::CORRECTING;
    } else {
        movingStateXY = MovingStateXY::IDLE;
    }
}

// MAQUINA DE ESTADOS PARA MOVIMIENTO XY-PLANE (MOVIMIENTO DE MOTORES)
// -----------------------------------------------------------------------
void updateXY() {
    switch (movingStateXY) {
        case MovingStateXY::IDLE:
            break;

        case MovingStateXY::MOVING_TO_TARGET:
            motor1.run();
            motor2.run();
            if (motor1.distanceToGo() == 0 &&
                motor2.distanceToGo() == 0) {
                settleStart = millis();
                delay(100); // estabilizacion mecanica
                Serial1.print("Moved to Target");
                // 🔥 AQUÍ VA LO IMPORTANTE
                currentShoulderAngle = targetShoulderAngle;
                currentElbowAngle = targetElbowAngle;
                // printDebugMove(targetShoulderAngle, targetElbowAngle);
                movingStateXY = MovingStateXY::SETTLING;
            }
            break;

        case MovingStateXY::SETTLING:
            if (millis() - settleStart > 100) {
                movingStateXY = MovingStateXY::CORRECTING;
                // Serial1.println("Correcting Error");
                correctErrorOnce();
            }
            break;

        case MovingStateXY::CORRECTING:
            motor1.run();
            motor2.run();
            if (motor1.distanceToGo() == 0 &&
                motor2.distanceToGo() == 0) {
                settleStart = millis();
                Serial1.println("Correction Done");
                Serial1.println();
                movingStateXY = MovingStateXY::IDLE;
            }
            break;
    }
}

// INICIAR SECUENCIA DE MOVIMIENTO COMPLETA (START → XY → Z → XY → Z)
// -----------------------------------------------------------------------
// startT1, startT2: ángulos iniciales para el movimiento XY
// endT1, endT2: ángulos finales para el movimiento XY
void startMoveSequence(float s1, float s2, float e1, float e2) {
    startT1 = s1;
    startT2 = s2;
    endT1 = e1;
    endT2 = e2;

    moveSeqState = MoveSequenceState::MOVING_START;

    moveToAngles(startT1, startT2);
}

// MAQUINA DE ESTADOS PARA SECUENCIA DE MOVIMIENTO COMPLETA (START → XY → Z → XY → Z)
// -----------------------------------------------------------------------
void updateMoveSequence() {
    switch (moveSeqState) {
        // 1. Mover pieza (START → XY)
        case MoveSequenceState::MOVING_START:

            if (!xyIsMoving()) {
                startZPick(); // 🔥 baja Z y agarra pieza
                moveSeqState = MoveSequenceState::PICKING;
            }
            break;

        // 2. Esperar Pick Terminado (Z)
        case MoveSequenceState::PICKING:

            if (movingStateZ == MovingStateZ::IDLE) {
                moveToAngles(endT1, endT2); // 🔥 ir a destino
                moveSeqState = MoveSequenceState::MOVING_END;
            }
            break;

        // 3. Movimiento final (END → XY)
        case MoveSequenceState::MOVING_END:

            if (!xyIsMoving()) {
                startZPlace(); // 🔥 soltar pieza
                moveSeqState = MoveSequenceState::PLACING;
            }
            break;

        // 4. Finalizado (esperar Place terminado)
        case MoveSequenceState::PLACING:

            if (movingStateZ == MovingStateZ::IDLE) {
                moveSeqState = MoveSequenceState::IDLE;
                Serial1.println("MOVE DONE");
            }
            break;

        // IDLE
        case MoveSequenceState::IDLE:
        default:
            break;
    }
}

// MOVIMIENTO A HOME DEL PLANO XY
// (NO ES HOMING SOLO REGRESAR A POSICION CERO DESPUES DEL MOVIMIENTO)
// -----------------------------------------------------------------------
void moveToHomeXY() {
    moveToAngles(0.0f, 0.0f);
}

// DEBUG
// -----------------------------------------------------------------------
void printDebugMove(float motor1Angle, float motor2Angle) {
    Serial1.println();
    Serial1.println("-------- MOTOR 1 --------");

    float sensor1 = estimateSensorAngle(
        targetShoulderAngle,
        motor1Config.reduction,
        sensor1Offset,
        motor1Config.motorDirection);

    Serial1.print("HomingOffset: ");
    Serial1.println(sensor1Offset, 1);

    Serial1.print("Estimated Sensor Angle: ");
    Serial1.println(sensor1, 1);

    Serial1.print("Real Sensor Angle: ");
    Serial1.println(rawToDegrees(sensorReadRawAngle(Wire)), 1);

    Serial1.println("-------- MOTOR 2 --------");

    float sensor2 = estimateSensorAngle(
        targetElbowAngle,
        motor2Config.reduction,
        sensor2Offset,
        motor2Config.motorDirection);

    Serial1.print("HomingOffset: ");
    Serial1.println(sensor2Offset, 1);

    Serial1.print("Estimated Sensor Angle: ");
    Serial1.println(round1Decimal(sensor2), 1);

    Serial1.print("Real Sensor Angle: ");
    Serial1.println(rawToDegrees(sensorReadRawAngle(Wire1)), 1);

    Serial1.println();

    float errorShoulder =
        calculateError(targetShoulderAngle, Wire, motor1Config, sensor1Offset);

    float errorElbow =
        calculateError(targetElbowAngle, Wire1, motor2Config, sensor2Offset);

    Serial1.print("Error1: ");
    Serial1.println(errorShoulder, 1);

    Serial1.print("Error2: ");
    Serial1.println(errorElbow, 1);

    Serial1.println();
}