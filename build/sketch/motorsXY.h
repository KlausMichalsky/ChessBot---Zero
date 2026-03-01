#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\motorsXY.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : motorsXY.h ❌ cambiar nombre a motors.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Declarar los motores utilizados por el sistema.
//      - Proveer funciones para habilitar y deshabilitar motores.
//      - Definir la API de movimiento y velocidad.
//      - Exponer funciones de servicio y parada de emergencia.
//      - Permitir la reutilización y escalabilidad del control
//        de motores a múltiples ejes.
// =======================================================================

#pragma once
#include <Arduino.h>
#include <AccelStepper.h>

// DECLARACIÓN DE MOTORES (EXTERN)
// =======================================================================
// Declaración externa de los objetos AccelStepper
// La palabra clave `extern` indica que estos objetos están
// definidos en otro archivo fuente (.cpp), sin duplicar instancias.
extern AccelStepper motor1;
extern AccelStepper motor2;
extern AccelStepper motor3;

// API PUBLICA FUNCIONES DE CONTROL DE MOTOR
// =======================================================================
// Inicialización
void motorsXY_Init();
void motorZ_Init();
// Control de energía
void motorsXY_Enable();
void motorsXY_Disable();
void motorZ_Enable();
void motorZ_Disable();

/*
// Movimiento
void motorsXY_Move(long x, long y);
void motorsXY_SetSpeed(long speed);

// Servicio (llamar en loop)
void motorsXY_Run();

// Emergencia
void motorsXY_Stop();

// Estado terminado?
bool motorsXY_Done();
*/