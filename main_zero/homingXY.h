// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : homingXY.h
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Declaraciones para el sistema de homing de motores paso a paso con
//      sensor Hall para movimientos angulares.
//      Define la estructura de estado y la API pública que
//      permite inicializar, arrancar, ejecutar y consultar el homing.
//      La máquina de estados y los valores de configuración se definen
//      en 'config.h'.
//  ▫️ RESPONSABILIDADES:
//      - Definir la estructura de estado del homing (HomingState).
//      - Proporcionar funciones públicas para inicializar y ejecutar la
//        rutina de homing.
//      - Permitir consultar el estado actual y detectar errores.
//      - Servir como interfaz para otros módulos del robot que requieran
//        funcionalidad de homing.
// =======================================================================
//  ▫️ ESTADO:
//      ⚠️ EN DESARROLLO ⚠️
//      Funcionalidad básica implementada y en fase de pruebas funcionales.
// =======================================================================

#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"

// ESTRUCTURA DE ESTADO DEL HOMING
// =======================================================================
// Esta estructura guarda TODO el estado necesario para ejecutar
// una rutina de homing no bloqueante.
// Se pasa por referencia entre homingXY_Init(), homingXY_Start(),
// homingXY_Step() y las funciones de consulta.
// Permite manejar múltiples motores con la misma lógica.
struct HomingState
{
    EstadoHoming state;      // Estado actual de la máquina de estados de homing
    unsigned long startTime; // Tiempo (millis) en el que comenzó el homing
    long firstEdge;          // Primer flanco detectado por el sensor
    long secondEdge;         // Segundo flanco detectado por el sensor
    long centerPosition;     // Posición calculada a partir de los flancos -> referencia absoluta
    bool fault;              // Flag de error latcheado, permanece activo hasta que el usuario lo resetea
};

// API PÚBLICA DEL MÓDULO DE HOMING
// =======================================================================
// Estas funciones forman la interfaz pública del módulo de homing.
// El código principal (loop) solo debe usar estas funciones,
// sin tocar estados internos ni lógica de la máquina de estados.
// Esto permite reutilizar el homing en distintos motores y proyectos.

// Inicializa el estado del homing
void homingXY_Init(HomingState &st);

// Inicia el proceso de homing
void homingXY_Start(AccelStepper &motor,
                    const HomingConfig &cfg,
                    HomingState &st,
                    int hallPin);

// Indica si el homing está actualmente en ejecución
bool homingXY_IsActive(const HomingState &st);

// Ejecuta un paso del homing (NO bloqueante)
void homingXY_Step(AccelStepper &motor,
                   const HomingConfig &cfg,
                   HomingState &st,
                   int hallPin);

// Indica si ocurrió un error durante el homing
bool homingXY_HasError(const HomingState &st);

// Devuelve el estado actual de la máquina de estados de homing
EstadoHoming homingXY_GetState(const HomingState &st);
