#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\command.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : command.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Declaración de funciones para la gestión de comandos
//        recibidos por UART
// =======================================================================

#include <Arduino.h>

#include "config.h"

#pragma once

bool commandAvailable();
String commandStatusReport();
void commandSendResponse(const String &msg);
void processCommand(const String &command);
String readCommand();
String commandReport(MotorID id);
