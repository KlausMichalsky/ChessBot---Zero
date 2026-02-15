#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\main_zero\\communication.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : communication.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definición de la interfaz de comunicación UART con el Raspberry
// =======================================================================

#include <Arduino.h>

#pragma once

#define DEBUG_UART 1 // 1 = debug activado, 0 = debug desactivado

void debug(const String &msg); // función para debug condicional
void UART_Init();
