#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\homingXY.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : homingXY.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN:
//      - Ejecutar la máquina de estados de homing.
//      - Detectar flancos del imán mediante sensor Hall.
//      - Calcular el centro del imán y posicionar motores en referencia.
//      - Gestionar errores y timeouts de homing.
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"
#include "homingXY.h"

// CONSTANTES INTERNAS DEL MÓDULO
// =======================================================================
// static → visibles solo dentro de este archivo (.cpp)
static const int8_t CW = 1;   // ClockWise  (sentido horario)
static const int8_t CCW = -1; // Counter-ClockWise (sentido antihorario)

// INICIALIZACIÓN DEL ESTADO DE HOMING
// =======================================================================
void homingXY_Init(HomingState &st)
{
    st.state = HOMING_INACTIVE; // estado inicial: homing apagado
    st.startTime = 0;           // timestamp de inicio de homing
    st.firstEdge = 0;           // primer flanco detectado (entrada/salida)
    st.secondEdge = 0;          // segundo flanco detectado
    st.centerPosition = 0;      // centro calculado entre flancos
    st.fault = false;           // no hay error latcheado
}

// INICIO DEL PROCESO DE HOMING
// =======================================================================
void homingXY_Start(AccelStepper &motor,
                    const HomingConfig &cfg,
                    HomingState &st,
                    int hallPin)
{
    if (st.state != HOMING_INACTIVE) // Evita reentradas: si el homing ya está activo, no hace nada
        return;
    pinMode(cfg.enablePin, OUTPUT);
    digitalWrite(cfg.enablePin, ENABLE_ACTIVE);

    pinMode(hallPin, INPUT_PULLUP);

    digitalWrite(LED, LOW);

    // Configuración dinámica del motor para homing y referencia temporal al iniciar homing
    motor.setMaxSpeed(cfg.fastSpeed);
    motor.setAcceleration(cfg.acceleration);
    motor.setCurrentPosition(0);

    // Inicializa variables internas del estado
    st.startTime = millis();          // marca de tiempo de inicio
    st.fault = false;                 // limpia error previo
    st.state = HOMING_SEARCH_FAST_CW; // primer estado del homing
}

// HOMING ACTIVO?
// =======================================================================
bool homingXY_IsActive(const HomingState &st)
{
    return st.state != HOMING_INACTIVE;
}

// MÁQUINA DE ESTADOS DE HOMING
// =======================================================================
void homingXY_Step(AccelStepper &motor,
                   const HomingConfig &cfg,
                   HomingState &st,
                   int hallPin)
{
    bool imanPresente = (digitalRead(hallPin) == LOW); // activo con pull-up, LOW = imán presente

    if (millis() - st.startTime > cfg.timeout) // si el homing dura más que cfg.timeout, marcar error
    {
        st.state = HOMING_ERROR;
    }

    switch (st.state)
    {
    case HOMING_SEARCH_FAST_CW:
        // 🔹 Mover rápido en sentido horario hasta detectar imán
        // o hasta alcanzar límite de 90° (cfg.steps90Deg)
        motor.setSpeed(CW * cfg.fastSpeed);
        motor.runSpeed();
        if (imanPresente)
            st.state = HOMING_FIND_FIRST_EDGE_CW; // imán detectado → buscar primer flanco
        else if (motor.currentPosition() > cfg.steps90Deg)
            st.state = HOMING_SEARCH_FAST_CCW; // no detectó imán → revertir dirección
        break;

    case HOMING_SEARCH_FAST_CCW:
        // 🔹 Mover rápido en sentido antihorario hasta detectar imán
        // o hasta límite de -90° (cfg.steps90Deg)
        motor.setSpeed(CCW * cfg.fastSpeed);
        motor.runSpeed();
        if (imanPresente)
            st.state = HOMING_FIND_FIRST_EDGE_CCW; // imán detectado → buscar primer flanco
        else if (motor.currentPosition() < -cfg.steps90Deg)
            st.state = HOMING_ERROR; // límite alcanzado en ambas direcciones → error
        break;

    case HOMING_FIND_FIRST_EDGE_CW:
        // 🔹 Aproximación lenta para detectar el primer flanco de salida del imán (caída)
        motor.setSpeed(CW * cfg.slowSpeed);
        motor.runSpeed();
        if (!imanPresente)
        {
            st.firstEdge = motor.currentPosition(); // guardar posición del primer flanco
            st.state = HOMING_REVERSE_EDGE_CW;      // invertir dirección para buscar segundo flanco
        }
        break;

    case HOMING_FIND_FIRST_EDGE_CCW:
        // 🔹 Aproximación lenta para detectar el primer flanco de salida del imán (caída) antihorario
        motor.setSpeed(CCW * cfg.slowSpeed);
        motor.runSpeed();
        if (!imanPresente)
        {
            st.firstEdge = motor.currentPosition();
            st.state = HOMING_REVERSE_EDGE_CCW;
        }
        break;

    case HOMING_REVERSE_EDGE_CW:
        // 🔹 Invertir dirección lentamente para encontrar el flanco de entrada del imán (subida)
        motor.setSpeed(CCW * cfg.slowSpeed);
        motor.runSpeed();
        if (imanPresente)
            st.state = HOMING_FIND_SECOND_EDGE_CW;
        break;

    case HOMING_REVERSE_EDGE_CCW:
        // 🔹 Invertir dirección lentamente para encontrar el flanco de entrada del imán (subida) antihorario
        motor.setSpeed(CW * cfg.slowSpeed);
        motor.runSpeed();
        if (imanPresente)
            st.state = HOMING_FIND_SECOND_EDGE_CCW;
        break;

    case HOMING_FIND_SECOND_EDGE_CW:
        // 🔹 Mover lentamente para detectar el segundo flanco de salida del imán
        motor.setSpeed(CCW * cfg.slowSpeed);
        motor.runSpeed();
        if (!imanPresente)
        {
            st.secondEdge = motor.currentPosition(); // guardar segundo flanco
            st.state = HOMING_CALC_CENTER;           // calcular el centro
        }
        break;

    case HOMING_FIND_SECOND_EDGE_CCW:
        // 🔹 Mover lentamente para detectar el segundo flanco de salida del imán antihorario
        motor.setSpeed(CW * cfg.slowSpeed);
        motor.runSpeed();
        if (!imanPresente)
        {
            st.secondEdge = motor.currentPosition();
            st.state = HOMING_CALC_CENTER;
        }
        break;

    case HOMING_CALC_CENTER:
        // 🔹 Calcular centro entre los dos flancos detectados y dar la orden de moverse al centro
        st.centerPosition = (st.firstEdge + st.secondEdge) / 2;
        motor.moveTo(st.centerPosition);
        st.state = HOMING_MOVE_TO_CENTER;
        break;

    case HOMING_MOVE_TO_CENTER:
        // 🔹 Moverse hasta la posición central (referencia)
        motor.run();
        if (motor.distanceToGo() == 0)
        {
            motor.setCurrentPosition(0); // definir posición cero
            digitalWrite(cfg.enablePin, ENABLE_INACTIVE);
            st.state = HOMING_OK;
        }
        break;

    case HOMING_OK:
        // 🔹 Homing completado correctamente
        digitalWrite(LED, HIGH); // indicar éxito
        digitalWrite(cfg.enablePin, ENABLE_INACTIVE);
        st.state = HOMING_INACTIVE; // reiniciar máquina de estados
        break;

    case HOMING_ERROR:
        // 🔹 Homing falló
        digitalWrite(cfg.enablePin, ENABLE_INACTIVE);
        st.fault = true;            // marcar error latcheado
        st.state = HOMING_INACTIVE; // reiniciar máquina de estados
        break;

    default:
        // 🔹 Si el estado es desconocido -> no hacer nada
        break;
    }
}

// PREGUNTAR ESTADO ACTUAL DEL HOMING
// =======================================================================
HomingStateEnum homingXY_GetState(const HomingState &st)
{
    return st.state; // devuelve el estado actual
}

// COMPROBAR SI HUBO ERROR EN HOMING
// =======================================================================
bool homingXY_HasError(const HomingState &st)
{
    return st.fault; // devuelve true si hubo error
}
