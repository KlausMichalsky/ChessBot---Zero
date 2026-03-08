// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : calc.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones de cálculo y conversiónes
// =======================================================================

#include <Arduino.h>
#include <math.h>

#include "communication.h"
#include "utils.h"

// Convierte valor bruto 12 bits del AS5600 (0-4095) a grados
float rawToDegrees(uint16_t rawAngle)
{
    return (rawAngle * 360.0) / 4096.0;
}

// Redondea un float a 1 decimal (ej. 123.456 -> 123.5)
float round1Decimal(float value)
{
    return ((int)(value * 10 + 0.5)) / 10.0;
}

// Filtra y envía un valor por UART solo si cambia suficiente y respeta intervalo
void sendFilteredFloat(float value, float &lastValue, unsigned long &lastTime,
                       float delta, unsigned long interval, HardwareSerial &uart)
{
    unsigned long now = millis();
    if (abs(value - lastValue) >= delta && now - lastTime >= interval)
    {
        uart.print(value, 1); // enviar valor redondeado
        uart.print("\n");
        lastValue = value;
        lastTime = now;
    }
}

/*
// Convierte coordenadas XY a ángulo relativo al eje (0-360°) ------------
float XYtoAngle(float x, float y)
{
    float angle = atan2(y, x) * 180.0 / PI; // atan2 devuelve en radianes, convertimos a grados
    if (angle < 0)
        angle += 360.0; // normalizamos a 0-360°
    return angle;
}
*/