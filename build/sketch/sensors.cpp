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

// VARIABLES INTERNAS DEL MÓDULO ❌ Mover estas variables a core❓
// -----------------------------------------------------------------------
// -1000 es solo un valor de “inicio imposible” para asegurar que el primer envío siempre se haga
unsigned long lastSendTime_1 = 0; // Guarda el momento en milisegundos del último envío
float lastSentAngle_1 = -1000.0f; // Guarda el último ángulo enviado
unsigned long lastSendTime_2 = 0;
float lastSentAngle_2 = -1000.0f;

float sensor1Offset = 0;
float sensor2Offset = 0;

// API PÚBLICA DE SENSORES
// -----------------------------------------------------------------------
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

// TwoWire → le decimos “la función va a recibir un objeto de tipo TwoWire”.
// &wire → le decimos “pasalo por referencia, no por copia”.
uint16_t sensorReadAngle(TwoWire &wire) {
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

// CALCULO DE OFFSET PARA CODIFICADOR AS5600
// -----------------------------------------------------------------------
// Calcula un promedio para reducir el ruido
float sensorHomingOffset(TwoWire &wire) {
    const uint8_t samples = 30;
    float sum = 0;
    float firstAngle = rawToDegrees(sensorReadAngle(wire));

    for (uint8_t i = 0; i < samples; i++) {
        float angle = rawToDegrees(sensorReadAngle(wire));
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

float sensorCorrectedAngle(TwoWire &wire, float offset) {
    float angle = rawToDegrees(sensorReadAngle(wire)) - offset;
    if (angle >= 360)
        angle -= 360;
    if (angle < 0)
        angle += 360;
    return angle;
}

// ⚠️ Manda angulo en grados al Serial solo para DEBUG 👀⁉️
void sensorSendAngle(TwoWire &wire) {
    uint16_t rawAngle = sensorReadAngle(wire); // Leer sensor AS5600
    float degrees = rawToDegrees(rawAngle);    // Convertir a grados
    // degrees = round1Decimal(degrees);          // Redondear a 1 decimal
    Serial1.print(degrees, 1); // Asegurar envio de solo 1 decimal
    Serial1.print("\n");
}