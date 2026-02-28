#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/commands.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : commands.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Declaración de funciones para la gestión de comandos
//        recibidos por UART
// =======================================================================

#pragma once
#include <Arduino.h>

bool commandAvailable();
String receiveCommand();
void sendResponse(const String &msg);
void processCommand(const String &command);
// void readAngles();
// void sendAngle(float angle);
// void sendStatus();
// void sendError(const String &error);
// void sendDebug(const String &debug);
