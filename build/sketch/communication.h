#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\communication.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : communication.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definición de la interfaz de comunicación UART
// =======================================================================

#include <Arduino.h>

#pragma once

#define DEBUG_UART 0 // 1 = debug activado, 0 = debug desactivado

void debug(const String &msg); // función para debug condicional
void communicationInit();
