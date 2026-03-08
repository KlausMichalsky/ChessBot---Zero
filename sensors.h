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

#include <Arduino.h>
#include <Wire.h>

// DECLARACIONES DE VARIABLES GLOBALES (extern -> sin duplicar instancias)
// -----------------------------------------------------------------------
extern float lastSentAngle_1;
extern unsigned long lastSendTime_1;
extern float lastSentAngle_2;
extern unsigned long lastSendTime_2;

void sensorsInit();
uint16_t readAngle(TwoWire &wire);
void sendDynamicAngle(TwoWire &wire, float &lastSentAngle, unsigned long &lastSendTime);
void sendStaticAngle(TwoWire &wire);
