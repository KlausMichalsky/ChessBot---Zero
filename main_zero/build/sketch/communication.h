#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\main_zero\\communication.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : communication.h
//  Autor      : Klaus Michalsky
//  Fecha      : 2026-02-01
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Interfaz de comunicación entre el controlador físico
//      y el cerebro lógico del sistema. Define las funciones para
//      inicializar la UART, recibir y procesar comandos, y enviar
//      estados, errores y mensajes de depuración.
//
//  ▫️ RESPONSABILIDADES:
//      - Inicializar la comunicación UART del sistema.
//      - Detectar la disponibilidad de comandos entrantes.
//      - Procesar comandos recibidos desde el cerebro lógico.
//      - Leer y enviar ángulos y datos relevantes del sistema.
//      - Enviar el estado actual del robot de forma estructurada.
//      - Reportar errores de ejecución.
//      - Proveer mensajes de depuración opcionales (apagables).
// =======================================================================
//  ▫️ ESTADO:
//      ⚠️ EN DESARROLLO ⚠️
//      Comunicación UART básica en implementación y pruebas.
//      Pruebas realizadas:
//      - Envío de mensajes simples por UART ✔️
//      Próximos pasos:
//      - Definición final del protocolo de comandos.
//      - Implementación no bloqueante del buffer de recepción.
//      - Integración completa con el parser del lado del cerebro lógico.
//      - Manejo de errores y estados extendidos.
//      - Desactivación de debug para versión de producción.
// =======================================================================

#pragma once
#include <Arduino.h>

// 🔹 FLAG DE DEBUG 🔹
#define DEBUG_UART 1 // 1 = debug activado, 0 = debug desactivado

void debug(const String &msg); // función para debug condicional
void UART_Init();
bool commandAvailable();
String receiveCommand();
void processCommand(const String &command);
void readAngles();
void sendAngle(float angle);
void sendStatus();
void sendError(const String &error);
void sendDebug(const String &debug);
