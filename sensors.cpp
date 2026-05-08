// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : sensors.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Inicializar y configurar sensores.
//      - Implementación de funciones para la lectura y gestión de
//        sensores AS5600.
// =======================================================================

#include <Arduino.h>

#include "config.h"
#include "sensors.h"
#include "utils.h"

// =============================================================
// OFFSETS (globales del sistema)
// =============================================================
float sensor1Offset = 0;
float sensor2Offset = 0;

float motor1Angle = 0;
float motor2Angle = 0;

float shoulderAngle = 0;
float elbowAngle = 0;

// =============================================================
// INIT SENSORES
// =============================================================
void sensorsInit() {
    pinMode(HALL_1, INPUT_PULLUP);
    pinMode(HALL_2, INPUT_PULLUP);
    pinMode(HALL_3, INPUT_PULLUP);

    Wire.setSDA(AS5600_1_SDA);
    Wire.setSCL(AS5600_1_SCL);
    Wire.begin();

    Wire1.setSDA(AS5600_2_SDA);
    Wire1.setSCL(AS5600_2_SCL);
    Wire1.begin();
}

// =============================================================
// RAW READ
// =============================================================
uint16_t sensorReadRawAngle(TwoWire &wire) {
    wire.beginTransmission(AS5600_ADDR);
    wire.write(0x0E);
    wire.endTransmission(false);
    wire.requestFrom(AS5600_ADDR, (uint8_t)2);

    if (wire.available() < 2)
        return 0;

    uint8_t high = wire.read();
    uint8_t low = wire.read();

    return ((high & 0x0F) << 8) | low;
}

// =============================================================
// HOMING OFFSET (CALIBRACIÓN)
// =============================================================
float sensorHomingOffset(TwoWire &wire) {
    const uint8_t samples = 30;
    float sum = 0;

    float firstAngle = rawToDegrees(sensorReadRawAngle(wire));

    for (uint8_t i = 0; i < samples; i++) {
        float angle = rawToDegrees(sensorReadRawAngle(wire));

        // corrección de salto 0/360
        if (fabs(angle - firstAngle) > 180) {
            if (angle < firstAngle)
                angle += 360;
            else
                angle -= 360;
        }

        sum += angle;
    }

    float offset = sum / samples;

    if (offset >= 360)
        offset -= 360;
    if (offset < 0)
        offset += 360;

    return offset;
}

// =============================================================
// CORRECTED ANGLE (0–360)
// =============================================================
float sensorCorrectedAngle(TwoWire &wire, float offset) {
    float angle = rawToDegrees(sensorReadRawAngle(wire)) - offset;

    if (angle >= 360)
        angle -= 360;
    if (angle < 0)
        angle += 360;

    return angle;
}

// =============================================================
// MULTI TURN (CLAVE DEL SISTEMA)
// =============================================================
float sensorMultiTurnAngle(TwoWire &wire, float offset) {
    static float lastAngle1 = 0;
    static float lastAngle2 = 0;

    static int turns1 = 0;
    static int turns2 = 0;

    float angle = sensorCorrectedAngle(wire, offset);

    float *lastAngle;
    int *turns;

    if (&wire == &Wire) {
        lastAngle = &lastAngle1;
        turns = &turns1;
    } else {
        lastAngle = &lastAngle2;
        turns = &turns2;
    }

    float delta = angle - *lastAngle;

    if (delta > 180)
        (*turns)--;
    if (delta < -180)
        (*turns)++;

    *lastAngle = angle;

    return angle + (*turns) * 360.0f;
}

void updateSensors() {
    // 🔥 lectura multivuelta (solo sensores aquí)
    motor1Angle = sensorMultiTurnAngle(Wire, sensor1Offset);
    motor2Angle = sensorMultiTurnAngle(Wire1, sensor2Offset);

    // 🔥 conversión a ángulo de brazo (cinemática básica)
    shoulderAngle = motor1Angle / motor1Config.reduction;
    elbowAngle = motor2Angle / motor2Config.reduction;
}