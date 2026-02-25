#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\calc.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : calc.h
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

// Convierte valor bruto del AS5600 (0-4095) a grados (0-360°)
float rawToDegrees(uint16_t rawAngle);

// Redondea un float a 1 decimal
float round1Decimal(float value);

// Filtra y envía un valor por UART solo si cambia suficiente y respeta intervalo
void sendFilteredFloat(float value, float &lastValue, unsigned long &lastTime,
                       float delta, unsigned long interval, HardwareSerial &uart);

// Convierte coordenadas XY a ángulo relativo al eje (en grados)
float XYtoAngle(float x, float y);