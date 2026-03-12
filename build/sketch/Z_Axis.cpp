#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\Z_Axis.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : ZAxis.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definicion de las funciones para controlar el eje Z
// =======================================================================

#include <Arduino.h>

#include "Z_Axis.h"
#include "config.h"
#include "homing.h"

void zDown() {
}

void zUp() {
}

void zPick() {
    zDown();
    delay(Z_DELAY);
    magnetON();
    zUp();
}

void zPlace() {
    zDown();
    delay(Z_DELAY);
    magnetOFF();
    zUp();
}