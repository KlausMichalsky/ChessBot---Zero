#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/sensors.h"
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

// Offsets calculados durante homing
extern float sensor1Offset;
extern float sensor2Offset;

// Ángulos ya corregidos con el offset
extern float sensor1Angle;
extern float sensor2Angle;

void sensorsInit();
uint16_t sensorReadAngle(TwoWire &wire);
void sensorStreamAngle(TwoWire &wire, float &lastSentAngle, unsigned long &lastSendTime);
void sensorSendAngle(TwoWire &wire);
float sensorHomingOffset(TwoWire &wire);
float sensorCorrectedAngle(TwoWire &wire, float offset);
