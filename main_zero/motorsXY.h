// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : motorsXY.h
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Interfaz de control de motores del robot de ajedrez.
//      Define las declaraciones de los motores y las funciones
//      de alto nivel para su inicialización, control de energía
//      y movimiento.
//
//  ▫️ RESPONSABILIDADES
//      - Declarar los motores utilizados por el sistema.
//      - Proveer funciones para habilitar y deshabilitar motores.
//      - Definir la API de movimiento y velocidad.
//      - Exponer funciones de servicio y parada de emergencia.
//      - Permitir la reutilización y escalabilidad del control
//        de motores a múltiples ejes.
// =======================================================================
//  ▫️ ESTADO:
//      ⚠️ EN DESARROLLO ⚠️
//      Funcionalidad básica implementada y en fase de pruebas funcionales.
// =======================================================================

#pragma once
#include <AccelStepper.h>

// DECLARACIÓN DE MOTORES (EXTERN)
// =======================================================================
// Declaración externa de los objetos AccelStepper utilizados
// para controlar los motores del sistema.
// La palabra clave `extern` indica que estos objetos están
// definidos en otro archivo fuente (.cpp), permitiendo
// compartir el control de los motores entre distintos
// módulos sin duplicar instancias.
extern AccelStepper motor1;
extern AccelStepper motor2;

// FUNCIONES DE CONTROL DE MOTOR
// =======================================================================
//   Conjunto de funciones de alto nivel para inicializar,
//   energizar y controlar el movimiento de un motor.
//   Estas funciones encapsulan:
//   - Inicialización del hardware y parámetros básicos.
//   - Habilitación y deshabilitación de potencia.
//   - Movimiento y control de velocidad.
//   - Ejecución continua del motor (servicio en loop).
//   - Parada de emergencia.
//   - Consulta del estado de movimiento.
//   Diseñado para ser reutilizado y extendido a múltiples
//   motores en etapas posteriores del proyecto.
// Inicialización
void motorsXY_Init();

// Control de energía
void motorsXY_Enable();
void motorsXY_Disable();
// Movimiento
void motorsXY_Move(long x, long y);
void motorsXY_SetSpeed(long speed);

// Servicio (llamar en loop)
void motorsXY_Run();

// Emergencia
void motorsXY_Stop();

// Estado terminado?
bool motorsXY_Done();