// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : motors.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Crear las instancias de los motores AccelStepper.
//      - Gestionar el estado interno de habilitación.
//      - Configurar velocidad y aceleración.
//      - Ejecutar movimientos coordinados en X e Y.
//      - Proveer parada segura y control de emergencia.
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include "motors.h"
#include "config.h"

// INSTANCIAS DE MOTORES
// =======================================================================
AccelStepper motor1(AccelStepper::DRIVER, MOTOR1_STEP, MOTOR1_DIR);
AccelStepper motor2(AccelStepper::DRIVER, MOTOR2_STEP, MOTOR2_DIR);
AccelStepper motor3(AccelStepper::DRIVER, MOTOR3_STEP, MOTOR3_DIR);

// ESTADO INTERNO DE MOTORES
// =======================================================================
static bool motorsXY_Enabled = false;
static bool motorZ_Enabled = false;

// API PÚBLICA DE MOTORES
// =======================================================================
void motors_Init()
{
    // ‼️ Adaptar también en config.h las señales ENABLE_ACTIVE/INACTIVE
    motor1.setPinsInverted(true, false, false); // (DIR, STEP, ENABLE) true = invertir señal. Aqui: LOW=ON HIGH=OFF
    motor2.setPinsInverted(true, false, false); // (DIR, STEP, ENABLE) true = invertir señal. Aqui: LOW=ON HIGH=OFF

    pinMode(motor1Config.enablePin, OUTPUT);
    pinMode(motor2Config.enablePin, OUTPUT);

    motorsXY_Disable(); // ‼️ Esto evita movimientos inesperados al encender el sistema

    // Configuración de parámetros básicos de cada motor
    motor1.setMaxSpeed(motor1Config.fastSpeed);
    motor1.setAcceleration(motor1Config.acceleration);

    motor2.setMaxSpeed(motor2Config.fastSpeed);
    motor2.setAcceleration(motor2Config.acceleration);
}

void motorsXY_Enable()
{
    digitalWrite(motor1Config.enablePin, ENABLE_ACTIVE);
    digitalWrite(motor2Config.enablePin, ENABLE_ACTIVE);

    // Actualiza el estado interno indicando que los motores
    // están habilitados y listos para recibir comandos de movimiento
    motorsXY_Enabled = true;
}

void motorsXY_Disable()
{
    digitalWrite(motor1Config.enablePin, ENABLE_INACTIVE);
    digitalWrite(motor2Config.enablePin, ENABLE_INACTIVE);
    motorsXY_Enabled = false;
}

void motorZ_Init()
{
    motor3.setPinsInverted(true, false, false);
    pinMode(motor3Config.enablePin, OUTPUT);
    motorsXY_Disable(); // ‼️ Esto evita movimientos inesperados al encender el sistema
    motor3.setMaxSpeed(motor3Config.fastSpeed);
    motor3.setAcceleration(motor3Config.acceleration);
}

void motorZ_Enable()
{
    digitalWrite(motor3Config.enablePin, ENABLE_ACTIVE);
    motorZ_Enabled = true;
}

void motorZ_Disable()
{
    digitalWrite(motor3Config.enablePin, ENABLE_INACTIVE);
    motorZ_Enabled = false;
}

/*
void motorsXY_Move(long x, long y)
{
    // No ejecuta el movimiento inmediatamente.
    // motorRun() debe llamarse repetidamente en loop()
    if (!motorsXY_Enabled)
        motorsXY_Enable();
    motor1.moveTo(x);
    motor2.moveTo(y);
}

void motorsXY_SetSpeed(long speed)
{
    motor1.setMaxSpeed(speed);
    motor2.setMaxSpeed(speed);
}

void motorsXY_Run()
{
    if (!motorsXY_Enabled)
        return; // 🔒 motor apagado = no ejecutar movimiento
    motor1.run();
    motor2.run();
}

void motorsXY_Stop()
{
    motor1.stop();
    motor2.stop();
    motorsXY_Disable(); // 🔥 CLAVE: apaga físicamente
}

bool motorsXY_Done()
{
    // Devuelve true si **ambos motores han llegado** a su posición objetivo
    return motor1.distanceToGo() == 0 &&
           motor2.distanceToGo() == 0;
}
*/