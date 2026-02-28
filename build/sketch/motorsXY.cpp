#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\motorsXY.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : motorsXY.cpp
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
#include "motorsXY.h"
#include "config.h"

// INSTANCIAS DE MOTORES
// =======================================================================
AccelStepper motor1(AccelStepper::DRIVER, MOTOR1_STEP, MOTOR1_DIR);
AccelStepper motor2(AccelStepper::DRIVER, MOTOR2_STEP, MOTOR2_DIR);

// ESTADO INTERNO DE MOTORES
// =======================================================================
static bool motorsEnabled = false;

// API PÚBLICA DE MOTORES
// =======================================================================
void motorsXY_Init()
{
    pinMode(HALL_1, INPUT_PULLUP);
    pinMode(HALL_2, INPUT_PULLUP);

    // ‼️ Adaptar también en config.h las señales ENABLE_ACTIVE/INACTIVE (niveles lógicos de enable)
    motor1.setPinsInverted(true, false, false); // (DIR, STEP, ENABLE) true = invertir señal. Aqui: LOW=ON HIGH=OFF
    motor2.setPinsInverted(true, false, false); // (DIR, STEP, ENABLE) true = invertir señal. Aqui: LOW=ON HIGH=OFF

    pinMode(motor1Config.enablePin, OUTPUT);
    pinMode(motor2Config.enablePin, OUTPUT);

    // ‼️ Esto evita movimientos inesperados al encender el sistema
    motorsXY_Disable();

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
    motorsEnabled = true;
}

void motorsXY_Disable()
{
    digitalWrite(motor1Config.enablePin, ENABLE_INACTIVE);
    digitalWrite(motor2Config.enablePin, ENABLE_INACTIVE);
    motorsEnabled = false;
}

/*
void motorsXY_Move(long x, long y)
{
    // ‼️ No ejecuta el movimiento inmediatamente.
    //    motorRun() debe llamarse repetidamente en loop()
    if (!motorsEnabled)
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
    if (!motorsEnabled)
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