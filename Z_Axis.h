// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : z_axis.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Declaracion de funciones para mover el eje Z
// =======================================================================

#include <Arduino.h>

#include "config.h"

#pragma once

// estado global del Z
extern MovingStateZ movingStateZ;

void zMoveDown();
void zMoveUp();
void magnetON();
void magnetOFF();
void startZPick();
void startZPlace();
void updateZ();