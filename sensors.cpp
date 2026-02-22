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

unsigned long lastSendTime = 0;  // Guarda el momento en milisegundos del último envío
float lastSentAngle1 = -1000.0f; // Guarda el último ángulo enviado para el motor 1
// -1000 es solo un valor de “inicio imposible” para asegurar que el primer envío siempre se haga

void sensorsInit()
{
    Wire.setSDA(AS5600_1_SDA);
    Wire.setSCL(AS5600_1_SCL);
    Wire.begin();
    // Aquí podrías agregar más inicializaciones si es necesario
}

// FUNCION AS5600 ----------------------------------------------------------------
uint16_t readAngle_1()
{
    Wire.beginTransmission(AS5600_ADDR);
    Wire.write(0x0E);
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDR, (uint8_t)2);

    if (Wire.available() < 2)
        return 0;

    uint8_t high = Wire.read();
    uint8_t low = Wire.read();

    return ((high & 0x0F) << 8) | low;
}

void sendAngle_1()
{
    uint16_t rawAngle = readAngle_1();
    float degrees = rawToDegrees(rawAngle);
    degrees = round1Decimal(degrees);
    sendFilteredFloat(degrees, lastAngle1, lastAngle1Time, 0.5, 1000, Serial1);
}
// Aquí podrías agregar funciones para leer otros sensores
// uint16_t readAS5600Angle_2()
// bool leerHall_2() { return digitalRead(HALL_2); }
// bool leerHall_1() { return digitalRead(HALL_1); }
