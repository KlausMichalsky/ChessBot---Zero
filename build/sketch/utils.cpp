#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/utils.cpp"
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
#include <cctype>  // para funciones de manipulación de caracteres (tolower), convertir mayúsculas/minúsculas verificar si es número, letra, etc.
#include <cstring> // para trabajar con strings tipo C (arrays de char).
#include <math.h>  //

#include "communication.h"
#include "config.h"
#include "utils.h"

// Convierte el angulo a pasos para cada motor
// -----------------------------------------------------------------------
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
// -----------------------------------------------------------------------
float rawToDegrees(uint16_t rawAngle) {
    return (rawAngle * 360.0) / 4096.0;
}

// Redondea un float a 1 decimal (ej. 123.456 -> 123.5)
// -----------------------------------------------------------------------
float round1Decimal(float value) {
    return ((int)(value * 10 + 0.5)) / 10.0;
}

// Filtra y envía un valor por UART solo si cambia suficiente y respeta intervalo
// -----------------------------------------------------------------------
// ⚠️ Solo para pruebas de lectura de angulo continuo
// -> El envio continuo bloquea movimiento de motores
void sendFilteredFloat(float value, float &lastValue, unsigned long &lastTime,
                       float delta, unsigned long interval, HardwareSerial &uart) {
    unsigned long now = millis();
    if (abs(value - lastValue) >= delta && now - lastTime >= interval) {
        uart.print(value, 1); // enviar valor redondeado
        uart.print("\n");
        lastValue = value;
        lastTime = now;
    }
}

// Normaliza ángulos entre -180 y +180
// -----------------------------------------------------------------------
// se aplica al error para evitar el wrap 0°/360°
float normalizeAngle(float angle) {
    while (angle > 180.0f)
        angle -= 360.0f;
    while (angle < -180.0f)
        angle += 360.0f;
    return angle;
}

// Convierte coordenadas de tablero (e.g. "e4") a coordenadas XY en mm
// -----------------------------------------------------------------------
bool boardToXY(const char *square, float &x, float &y) {
    // Validación básica
    if (square == nullptr || strlen(square) != 2)
        return false;

    // Separar columna y fila
    char column = tolower(square[0]); // 'a' a 'h'
    char row = square[1];             // '1' a '8'

    // Validar rango
    if (column < 'a' || column > 'h')
        return false;
    if (row < '1' || row > '8')
        return false;

    // Convertir a índices (0–7)
    int columnIndex = column - 'a';
    int rowIndex = row - '1';

    // Convertir a coordenadas reales (centro de la casilla)
    x = ORIGIN_X + (columnIndex + 0.5f) * SQUARE_SIZE;
    y = ORIGIN_Y + (rowIndex + 0.5f) * SQUARE_SIZE;

    return true;
}

// CINEMÁTICA INVERSA
// -----------------------------------------------------------------------
// x, y → posición objetivo (mm)
// theta1, theta2 → resultados (ángulos)
// & → se modifican directamente afuera de la función
// bool → devuelve si el punto es alcanzable o no
bool inverseKinematics(float x, float y, float &theta1, float &theta2) {
    // Longitudes del brazo
    const float L1 = 175.0f;
    const float L2 = 95.0f;

    // Distancia al punto
    // r² = x² + y²
    //     ⚡ ¿Por qué usamos r2 primero?
    // Porque la raíz cuadrada (sqrt) es más “cara” en cálculo.
    // Entonces hacemos:
    // ❌ lento (si lo haces siempre)
    // r = sqrt(x*x + y*y);
    // ✅ optimizado (como lo hiciste tú)
    // r2 = x*x + y*y;   // sin raíz todavía
    // Y SOLO cuando necesitas el valor real:
    // r = sqrt(r2);
    // 🧠 Ejemplo mental fácil
    // Imagina:
    // x = 3
    // y = 4
    // Paso 1: cuadrado
    // r2 = 3² + 4²
    // r2 = 9 + 16 = 25
    // Paso 2: raíz
    // r = √25 = 5
    // 💡 ¿Por qué esto importa en tu robot?
    // Porque en esta línea:
    // if (r > (L1 + L2))
    // estás preguntando:
    // 👉 “¿el punto está demasiado lejos para el brazo?”
    // ⚠️ Idea clave
    // r2 = útil para cálculos rápidos
    // r = valor humano (distancia real)
    // 💬 Resumen directo
    // 👉 r2 = distancia al cuadrado (sin raíz, rápido)
    // 👉 r = distancia real (con raíz, más lento pero entendible)
    // 👉 usamos ambos porque IK lo necesita así
    float r2 = x * x + y * y;
    float r = sqrt(r2);

    // Verifica si el punto es alcanzable
    //  ❌ Muy lejos  r > L1 + L2
    //  ❌ Muy cerca  r < |L1 - L2|
    if (r > (L1 + L2) || r < fabs(L1 - L2))
        return false;

    // Cálculo de theta2 (ángulo del codo)
    // Esta es la ley del coseno para el ángulo del codo, qué tan doblado está el codo
    float cos_theta2 = (r2 - L1 * L1 - L2 * L2) / (2.0f * L1 * L2);

    // Clamp por seguridad numérica
    // evita errores numéricos
    // Porque:
    // acos(1.0000001) = error
    // acos(-1.0000001) = error
    if (cos_theta2 > 1.0f)
        cos_theta2 = 1.0f;
    if (cos_theta2 < -1.0f)
        cos_theta2 = -1.0f;

    // convierte coseno → ángulo real
    // 🔥 RADIANES (NO convertir aquí)
    float theta2_rad = acos(cos_theta2);

    // esto simplifica el cálculo del hombro
    // k1 = componente horizontal del brazo
    // k2 = componente vertical del segundo segmento
    float k1 = L1 + L2 * cos(theta2_rad);
    float k2 = L2 * sin(theta2_rad);

    // Cálculo de theta1 (ángulo del hombro)
    // atan2(y, x) ángulo directo hacia el punto
    // atan2(k2, k1)
    // corrección por la forma del brazo
    // resultado final:
    // “hacia dónde apuntar menos cómo está doblado el brazo”
    float theta1_rad = atan2(y, x) - atan2(k2, k1);

    // 🔥 SOLO AQUÍ conviertes a grados
    theta1 = theta1_rad * 180.0f / M_PI;
    theta2 = theta2_rad * 180.0f / M_PI;

    // todo funcionó, el punto es alcanzable
    return true;
}