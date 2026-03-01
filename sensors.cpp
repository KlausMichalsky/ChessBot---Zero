// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : sensors.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones para la lectura y gestión de
//        sensores AS5600.
//      - Inicializar y configurar sensores.
//      - Leer valores.
//      - Validar y filtrar datos obtenidos.
// =======================================================================

// sensores.cpp
#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "calc.h"

unsigned long lastSendTime_1 = 0; // Guarda el momento en milisegundos del último envío
float lastSentAngle_1 = -1000.0f; // Guarda el último ángulo enviado para el motor 1
unsigned long lastSendTime_2 = 0; // Guarda el momento en milisegundos del último envío para el motor 2
float lastSentAngle_2 = -1000.0f; // Guarda el último á
// -1000 es solo un valor de “inicio imposible” para asegurar que el primer envío siempre se haga

void sensors_Init()
{
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

// FUNCION AS5600 ----------------------------------------------------------------
// TwoWire → le decimos “la función va a recibir un objeto de tipo TwoWire”.
// &wire → le decimos “pasalo por referencia, no por copia”.
uint16_t readAngle(TwoWire &wire)
{
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

void sendStaticAngle(TwoWire &wire)
{
    uint16_t rawAngle = readAngle(wire);    // Leer sensor AS5600
    float degrees = rawToDegrees(rawAngle); // Convertir a grados
    degrees = round1Decimal(degrees);       // Redondear a 1 decimal
    Serial1.print(degrees, 1);              // Enviar por UART
    Serial1.print("\n");
}

void sendDynamicAngle(TwoWire &wire, float &lastSentAngle, unsigned long &lastSendTime)
{
    uint16_t rawAngle = readAngle(wire);                                       // Leer sensor AS5600
    float degrees = rawToDegrees(rawAngle);                                    // Convertir a grados
    degrees = round1Decimal(degrees);                                          // Redondear a 1 decimal
    sendFilteredFloat(degrees, lastSentAngle, lastSendTime, 0.5, 33, Serial1); // Enviar por UART
}

// Aquí podrías agregar funciones para leer otros sensores
// uint16_t readAS5600Angle_2()
// bool leerHall_2() { return digitalRead(HALL_2); }
// bool leerHall_1() { return digitalRead(HALL_1); }
