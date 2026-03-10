#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\motors.cpp"
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
//      - Ejecutar movimientos coordinados en el plano XY y eje Z.
//      - Proveer parada segura y control de emergencia.
// =======================================================================

#include <Arduino.h>

#include <AccelStepper.h>

#include "config.h"
#include "motors.h"

// INSTANCIAS DE MOTORES
// -----------------------------------------------------------------------
AccelStepper motor1(AccelStepper::DRIVER, MOTOR1_STEP, MOTOR1_DIR);
AccelStepper motor2(AccelStepper::DRIVER, MOTOR2_STEP, MOTOR2_DIR);
AccelStepper motor3(AccelStepper::DRIVER, MOTOR3_STEP, MOTOR3_DIR);

// ESTADO INTERNO DE MOTORES
// -----------------------------------------------------------------------
static bool motorsEnabledXY = false;
static bool motorEnabledZ = false;

// API PÚBLICA DE MOTORES
// -----------------------------------------------------------------------
void motorsInit() {
    // ‼️ Adaptar también en config.h las señales ENABLE_ACTIVE/INACTIVE
    // (DIR, STEP, ENABLE) true = invertir señal. Aqui: LOW=ON HIGH=OFF
    motor1.setPinsInverted(true, false, false);
    motor2.setPinsInverted(true, false, false);
    motor3.setPinsInverted(false, false, false);

    pinMode(motor1Config.enablePin, OUTPUT);
    pinMode(motor2Config.enablePin, OUTPUT);
    pinMode(motor3Config.enablePin, OUTPUT);

    // ‼️ Esto evita movimientos inesperados al encender el sistema
    motorsDisableXY();
    motorDisableZ();

    motor1.setMaxSpeed(motor1Config.fastSpeed);
    motor2.setMaxSpeed(motor2Config.fastSpeed);
    motor3.setMaxSpeed(motor3Config.fastSpeed);

    motor1.setAcceleration(motor1Config.acceleration);
    motor2.setAcceleration(motor2Config.acceleration);
    motor3.setAcceleration(motor3Config.acceleration);
}

void motorsEnableXY() {
    digitalWrite(motor1Config.enablePin, ENABLE_ACTIVE);
    digitalWrite(motor2Config.enablePin, ENABLE_ACTIVE);
    motorsEnabledXY = true;
}

void motorEnableZ() {
    digitalWrite(motor3Config.enablePin, ENABLE_ACTIVE);
    motorEnabledZ = true;
}

void motorsDisableXY() {
    digitalWrite(motor1Config.enablePin, ENABLE_INACTIVE);
    digitalWrite(motor2Config.enablePin, ENABLE_INACTIVE);
    motorsEnabledXY = false;
}

void motorDisableZ() {
    digitalWrite(motor3Config.enablePin, ENABLE_INACTIVE);
    motorEnabledZ = false;
}

const char *motorName(MotorID id) {
    switch (id) {
        case MotorID::J1:
            return "J1";
        case MotorID::J2:
            return "J2";
        case MotorID::Z:
            return "Z";
        default:
            return "?"; // devuelve ? si no coincide ningun valor
    }
}
