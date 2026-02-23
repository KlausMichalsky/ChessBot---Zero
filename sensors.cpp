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
// -1000 es solo un valor de “inicio imposible” para asegurar que el primer envío siempre se haga

void sensorsInit()
{
    Wire1.setSDA(AS5600_2_SDA);
    Wire1.setSCL(AS5600_2_SCL);
    Wire1.begin();
    // Aquí podrías agregar más inicializaciones si es necesario
}

// FUNCION AS5600 ----------------------------------------------------------------
uint16_t readAngle_1()
{
    Wire1.beginTransmission(AS5600_ADDR);
    Wire1.write(0x0E);
    Wire1.endTransmission(false);
    Wire1.requestFrom(AS5600_ADDR, (uint8_t)2);

    if (Wire1.available() < 2)
        return 0;

    uint8_t high = Wire1.read();
    uint8_t low = Wire1.read();

    return ((high & 0x0F) << 8) | low;
}

// float convertRawToDegrees_1(uint16_t rawAngle)
// {
//     uint16_t rawAngle_1 = readAngle_1();
//     float degrees_1 = rawToDegrees(rawAngle);
//     degrees_1 = round1Decimal(degrees_1);
//     return degrees_1;
// }

void sendAngle_1()
{
    uint16_t rawAngle_1 = readAngle_1();                                             // Leer sensor AS5600
    float degrees_1 = rawToDegrees(rawAngle_1);                                      // Convertir a grados
    degrees_1 = round1Decimal(degrees_1);                                            // Redondear a 1 decimal
    sendFilteredFloat(degrees_1, lastSentAngle_1, lastSendTime_1, 0.5, 33, Serial1); // Enviar por UART
}
// Aquí podrías agregar funciones para leer otros sensores
// uint16_t readAS5600Angle_2()
// bool leerHall_2() { return digitalRead(HALL_2); }
// bool leerHall_1() { return digitalRead(HALL_1); }
