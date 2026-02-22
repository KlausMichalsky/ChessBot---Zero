#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\sensors.h"
// =======================================================================
//                    🔹 P I C O   —   C H E S S 🔹
// =======================================================================
//  Archivo    : sensores.h
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Declaración de funciones y constantes para el manejo
//      de sensores del robot de ajedrez.
//  ▫️ RESPONSABILIDADES:
//      - Proveer prototipos de funciones de inicialización y lectura.
//      - Definir constantes y macros para sensores.
//      - Servir de interfaz clara para el resto del proyecto.
// =======================================================================

#pragma once
#include <Wire.h>

void sensorsInit();
uint16_t readAngle_1();
float convertRawToDegrees_1(uint16_t rawAngle);
void sendAngle_1();
