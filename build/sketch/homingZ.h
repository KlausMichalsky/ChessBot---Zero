#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/homingZ.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : homingZ.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definir la estructura de estado del homing (HomingRunTimeZ).
//      - Proporcionar funciones públicas para inicializar y ejecutar la
//        rutina de homing.
//      - Permitir consultar el estado actual y detectar errores.
//      - Servir como interfaz para otros módulos del robot que requieran
//        funcionalidad de homing.
// =======================================================================

#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"

// ESTRUCTURA DE ESTADO DEL HOMING
// =======================================================================
// Esta estructura guarda TODO el estado necesario para ejecutar
// una rutina de homing no bloqueante.
// Se pasa por referencia entre homingZ_Init(), homingZ_Start(),
// homingZ_Step() y las funciones de consulta.
// Permite manejar múltiples motores con la misma lógica.
struct HomingRunTimeZ
{
    HomingStateZ state;      // Estado actual de la máquina de estados de homing// <-- aquí usamos el enum
    unsigned long startTime; // Tiempo (millis) en el que comenzó el homing
    long initialPosition;    // Posición calculada a partir de los flancos -> referencia absoluta
    long edge;               // Flanco de salida detectado por el sensor
    long reference;          // Posiciónde homing calculada
    bool fault;              // Flag de error latcheado, permanece activo hasta que el
};

// API PÚBLICA DEL MÓDULO DE HOMING
// =======================================================================
// Inicializa el estado del homing
void homingZ_Init(HomingRunTimeZ &st);

// Inicia el proceso de homing
void homingZ_Start(AccelStepper &motor,
                   const HomingConfig &cfg,
                   HomingRunTimeZ &st,
                   int hallPin);

// Indica si el homing está actualmente en ejecución
bool homingZ_IsActive(const HomingRunTimeZ &st);

// Ejecuta un paso del homing (NO bloqueante)
void homingZ_Step(AccelStepper &motor,
                  const HomingConfig &cfg,
                  HomingRunTimeZ &st,
                  int hallPin);

// Indica si ocurrió un error durante el homing
bool homingZ_HasError(const HomingRunTimeZ &st);

// Devuelve el estado actual del homing
HomingStateZ homingZ_GetState(const HomingRunTimeZ &st);
