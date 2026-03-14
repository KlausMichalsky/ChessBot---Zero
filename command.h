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

#pragma once

#include <Arduino.h>

#include "config.h"

bool commandAvailable();
String commandStatusReport();
void commandSendResponse(const String &msg);
void processCommand(const String &command);
String readCommand();
void commandSendStatusReport();
