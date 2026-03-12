#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\Z_Axis.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : ZAxis.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Control del eje Z para subir y bajar
//      - Ejecutar ciclo completo de pick
// =======================================================================

#include <Arduino.h>

#pragma once

// ESTRUCTURA DE ESTADO DE PICK AND PLACE
// -----------------------------------------------------------------------
struct PickPlaceZ {
    // HomingStateZ state;      // Estado actual de la máquina de estados de homing// <-- aquí usamos el enum
    // unsigned long startTime; // Tiempo (millis) en el que comenzó el homing
    // long initialPosition;    // Posición calculada a partir de los flancos -> referencia absoluta
    // long edge;               // Flanco de salida detectado por el sensor
    // long reference;          // Posiciónde homing calculada
    // bool fault;              // Flag de error latcheado, permanece activo hasta que el
};

void zDown();
void zUp();
void zPick();
void zPlace();
void magnetON();
void magnetOFF();