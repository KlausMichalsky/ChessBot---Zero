#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\utils.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : utils.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones de cálculo y conversiónes
// =======================================================================

#include <Arduino.h>
#include <math.h>

#include "communication.h"
#include "config.h"
#include "sensors.h"
#include "utils.h"

// Convierte el angulo a pasos para cada motor
long angleToStep(float angle, MotorID id) {
    switch (id) {
        case MotorID::J1:
            return (angle / 360.0) *
                   motor1Config.microstepping *
                   motor1Config.reduction *
                   motor1Config.stepsPerRevolution;
        case MotorID::J2:
            return (angle / 360.0) *
                   motor2Config.microstepping *
                   motor2Config.reduction *
                   motor2Config.stepsPerRevolution;
        default:
            // Por si llega un MotorID inválido
            Serial1.println("ERROR: MotorID inválido en angleToStep");
            return 0;
    }
}

// Convierte valor bruto 12 bits del AS5600 (0-4095) a grados
float rawToDegrees(uint16_t rawAngle) {
    return (rawAngle * 360.0) / 4096.0;
}

// Redondea un float a 1 decimal (ej. 123.456 -> 123.5)
float round1Decimal(float value) {
    return ((int)(value * 10 + 0.5)) / 10.0;
}

// Filtra y envía un valor por UART solo si cambia suficiente y respeta intervalo
// ⚠️ Solo para pruebas de lectura de angulo continuo
// -> El envio continuo bloquea movimiento de motores
// void sendFilteredFloat(float value, float &lastValue, unsigned long &lastTime,
//                        float delta, unsigned long interval, HardwareSerial &uart) {
//     unsigned long now = millis();
//     if (abs(value - lastValue) >= delta && now - lastTime >= interval) {
//         uart.print(value, 1); // enviar valor redondeado
//         uart.print("\n");
//         lastValue = value;
//         lastTime = now;
//     }
// }

// Normaliza ángulos entre -180 y +180
// se aplica al error para evitar el wrap 0°/360°
float normalizeAngle(float angle) {
    while (angle > 180.0f)
        angle -= 360.0f;
    while (angle < -180.0f)
        angle += 360.0f;
    return angle;
}

// Calcula la posicion estimada del sensor posicion teorica
// osea por ejemplo:
// targetAngle es 30° entonces
// 30°*reduccion el motor da varias vueltas y el sensor queda siempre en un
// angulo entre 0 y 360 y a ese angulo hay que sumarle el offset del homing
float calculateJointAngle(float target, float offset, float reduction) {
    float result = (target * reduction) + offset;

    float finalAngle = fmod(result, 360.0f);
    if (finalAngle < 0)
        finalAngle += 360.0f;

    return finalAngle;
}

float angleError(float target, float offset, float reduction, TwoWire &wire) {
    float estimated = calculateJointAngle(target, offset, reduction);
    float actual = rawToDegrees(sensorReadAngle(wire));

    float error = actual - estimated;

    // normalizar a rango [-180, 180]
    while (error > 180.0f)
        error -= 360.0f;
    while (error < -180.0f)
        error += 360.0f;

    return error;
}
