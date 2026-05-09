#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\sensors.cpp"
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
// Variables (globales del sistema)
// =============================================================
float sensor1Offset = 0;
float sensor2Offset = 0;

float motor1Angle = 0;
float motor2Angle = 0;

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

// Recomendacion para la funcion estimateSensorAngle
// 👉enum Direction {
//     CW = 1,
//     CCW = -1
// };

float estimateSensorAngle(
    float targetAngle,
    float reduction,
    float homingOffset,
    bool invertMotor,
    bool invertSensor) {
    float motorDir = invertMotor ? -1.0f : 1.0f;
    float sensorDir = invertSensor ? -1.0f : 1.0f;
    // ejemplo
    // si invertMotor = true  -> motorDir = -1
    // si invertMotor = false -> motorDir =  1

    // 🔥 ángulo motor real
    float motorAngle = motorDir * targetAngle * reduction;
    // ejemplo motorAngle = 1 * 90 * 30 = 2700°

    // Serial1.print("motorAngle: ");
    // Serial1.println(motorAngle);

    // 🔥 normalizar vueltas
    // El operador módulo % o fmod() devuelve el “sobrante”
    float rest = fmod(motorAngle, 360.0f);
    // ejemplo rest = fmod(2700, 360) = 180

    // Serial1.print("rest: ");
    // Serial1.println(rest);

    // Convierte negativos a rango positivo
    if (rest < 0)
        rest += 360.0f;

    // Serial1.print("fmodRest: ");
    // Serial1.println(rest);

    // 🔥 reconstrucción sensor
    float estimatedSensorAngle = sensorDir * rest + homingOffset;

    // Serial1.print("estimatedSensorAngle: ");
    // Serial1.println(estimatedSensorAngle);

    // 🔥 wrap final verifica si pasó de 360° o quedo negativo
    estimatedSensorAngle = fmod(estimatedSensorAngle, 360.0f);
    if (estimatedSensorAngle < 0)
        estimatedSensorAngle += 360.0f;

    // Serial1.print("fmodEstimatedSensorAngle: ");
    // Serial1.println(estimatedSensorAngle);

    return estimatedSensorAngle;
}