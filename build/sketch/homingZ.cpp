#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\homingZ.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : homingZ.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN:
//      - Ejecutar la máquina de estados de homing del motor3 (Z).
//      - Detectar flancos del imán mediante sensor Hall.
//      - Calcular el centro del imán y posicionar motores en referencia.
//      - Gestionar errores y timeouts de homing.
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"
#include "homingZ.h"
#include "sensors.h"

// CONSTANTES INTERNAS DEL MÓDULO
// =======================================================================
// static → visibles solo dentro de este archivo (.cpp)
static const int8_t dir = 1; // Dirección inicial

// INICIALIZACIÓN DEL ESTADO DE HOMING
// =======================================================================
void homingZ_Init(HomingRunTimeZ &st)
{
    st.state = HomingStateZ::INACTIVE; // estado inicial: homing apagado
    st.startTime = 0;                  // timestamp de inicio de homing
    st.edge = 0;                       // flanco detectado (entrada/salida)
    st.reference = 0;                  // referencia calculada
    st.fault = false;                  // no hay error latcheado
}

// INICIO DEL PROCESO DE HOMING
// =======================================================================
void homingZ_Start(AccelStepper &motor,
                   const HomingConfig &cfg,
                   HomingRunTimeZ &st,
                   int hallPin)
{
    if (st.state != HomingStateZ::INACTIVE) // Evita reentradas: si el homing ya está activo, no hace nada
        return;

    pinMode(cfg.enablePin, OUTPUT);
    digitalWrite(cfg.enablePin, ENABLE_ACTIVE);
    digitalWrite(LED, LOW);

    // Configuración dinámica del motor para homing y referencia temporal al iniciar homing
    motor.setMaxSpeed(cfg.fastSpeed);
    motor.setAcceleration(cfg.acceleration);
    motor.setCurrentPosition(0);

    // Inicializa variables internas del estado
    st.startTime = millis();                     // marca de tiempo de inicio
    st.fault = false;                            // limpia error previo
    st.state = HomingStateZ::FIND_EDGE_DOWNWARD; // primer estado del homing
}

// HOMING ACTIVO?
// =======================================================================
bool homingZ_IsActive(const HomingRunTimeZ &st)
{
    return st.state != HomingStateZ::INACTIVE;
}

// PREGUNTAR ESTADO ACTUAL DEL HOMING
// =======================================================================
HomingStateZ homingZ_GetState(const HomingRunTimeZ &st)
{
    return st.state; // devuelve el estado actual
}

// COMPROBAR SI HUBO ERROR EN HOMING
// =======================================================================
bool homingZ_HasError(const HomingRunTimeZ &st)
{
    return st.fault; // devuelve true si hubo error
}

// MÁQUINA DE ESTADOS DE HOMING
// =======================================================================
void homingZ_Step(AccelStepper &motor,
                  const HomingConfig &cfg,
                  HomingRunTimeZ &st,
                  int hallPin)
{
    // Invierte la logica del HAll (imán presente = LOW)
    bool imanPresente = (digitalRead(hallPin) == LOW); // activo con pull-up

    // ⏱️ Timeout de homing (si tiempo de homing excede el límite)
    if (millis() - st.startTime > cfg.timeout)
    {
        st.state = HomingStateZ::ERROR;
    }

    switch (st.state)
    {
    case HomingStateZ::FIND_EDGE_DOWNWARD:
        motor.setSpeed(dir * cfg.fastSpeed);
        motor.runSpeed();
        if (!imanPresente)
        {
            st.edge = motor.currentPosition();
            motor.moveTo(st.edge + 500); // avanza 500 pasos para alejarse un poquito del imán
            st.state = HomingStateZ::FIND_EDGE_UPWARD;
        }
        break;

    case HomingStateZ::FIND_EDGE_UPWARD:
        motor.setSpeed(-cfg.slowSpeed);
        motor.runSpeed();
        if (imanPresente)
        {
            st.edge = motor.currentPosition();
            st.state = HomingStateZ::MOVE_TO_REFERENCE;
        }
        // Probar esto:
        else if (motor.currentPosition() <= cfg.stepsLimit)
        {
            st.state = HomingStateZ::ERROR;
        }
        break;

    case HomingStateZ::MOVE_TO_REFERENCE:
        motor.setSpeed(-cfg.slowSpeed);
        motor.runSpeed();

        if (motor.currentPosition() <= st.edge - 500)
        {
            motor.stop();
            motor.setCurrentPosition(0); // ESTE es el cero (referencia)
            digitalWrite(cfg.enablePin, ENABLE_INACTIVE);
            st.state = HomingStateZ::OK;
        }
        break;

    case HomingStateZ::OK:
        digitalWrite(LED, HIGH);
        st.state = HomingStateZ::INACTIVE;
        // digitalWrite(cfg.enablePin, ENABLE_INACTIVE); ❌ quitar esta linea
        break;

    case HomingStateZ::ERROR:
        digitalWrite(cfg.enablePin, ENABLE_INACTIVE);
        st.fault = true; // marca la falla
        // st.state = HomingStateZ::INACTIVE; // ❌ quitar esta linea
        break;

    default:
        break;
    }
}