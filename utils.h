// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : utils.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Funciones de cálculo y conversión de valores
//      - Ángulos de sensores
//      - Coordenadas XY a ángulos para motores
// =======================================================================

#pragma once

#include <Arduino.h>

float rawToDegrees(uint16_t rawAngle);

float round1Decimal(float value);

void sendFilteredFloat(float value, float &lastValue, unsigned long &lastTime,
                       float delta, unsigned long interval, HardwareSerial &uart);
