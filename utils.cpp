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

// CONVERCION DE ANGULO A PASOS DE MOTOR
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

// CONVERCION DE VALOR BRUTO DE 12 BITS DEL AS5600 (0-4095) A GRADOS
// -----------------------------------------------------------------------
float rawToDegrees(uint16_t rawAngle) {
    return (rawAngle * 360.0) / 4096.0;
}

// REDONDEO DE FLOAT A 1 DECIMAL
// -----------------------------------------------------------------------
float round1Decimal(float value) {
    return ((int)(value * 10 + 0.5)) / 10.0;
}

// CONVERSIÓN DE CASILLA → XY
// -----------------------------------------------------------------------
bool chessSquareToXY(
    const String &square,
    float &x,
    float &y) {
    // VALIDAR LONGITUD
    if (square.length() != 2) { // Verifica que el texto tenga exactamente 2 caracteres.
        return false;
    }

    // OBTENER LETRA Y NÚMERO
    char file = toupper(square[0]); // Optener la primera letra y convertira mayuscula
    char rank = square[1];          // obtener el numero

    // VALIDAR RANGO
    if (file < 'A' || file > 'H') {
        return false;
    }
    if (rank < '1' || rank > '8') {
        return false;
    }

    // CONVERTIR A ÍNDICES
    int fileIndex = file - 'A';
    int rankIndex = rank - '1';

    // CONVERTIR A COORDENADAS CENTRO DE CASILLAS
    x = BOARD_OFFSET_X +
        fileIndex * SQUARE_SIZE +
        SQUARE_SIZE / 2;

    y = BOARD_OFFSET_Y +
        rankIndex * SQUARE_SIZE +
        SQUARE_SIZE / 2;

    return true;
}

// CONVERSIÓN XY → ANGULOS (CINEMATICA INVERSA)
// -----------------------------------------------------------------------
bool xyToIK(
    float x,
    float y,
    float L1,
    float L2,
    float &theta1,
    float &theta2) {
    float r2 = x * x + y * y;
    float r = sqrt(r2);

    // CHECK: punto alcanzable
    if (r > (L1 + L2) || r < fabs(L1 - L2)) {
        return false; // fuera del alcance
    }

    float cosTheta2 = (r2 - L1 * L1 - L2 * L2) / (2 * L1 * L2);
    theta2 = acos(cosTheta2);

    float k1 = L1 + L2 * cos(theta2);
    float k2 = L2 * sin(theta2);

    theta1 = atan2(y, x) - atan2(k2, k1);

    return true;
}

// DEBUG
// TEST: IMPRIMIR TODAS LAS CASILLAS COMO COORDENADAS XY
// -----------------------------------------------------------------------
void printBoardXY() {
    Serial1.println();
    Serial1.println("-------- BOARD XY DEBUG --------");

    for (char file = 'A'; file <= 'H'; file++) {
        for (char rank = '1'; rank <= '8'; rank++) {
            String square = String(file) + String(rank);

            float x, y;

            delay(10);

            if (chessSquareToXY(square, x, y)) {
                Serial1.print(square);
                Serial1.print(" -> X: ");
                Serial1.print(x);
                Serial1.print(" Y: ");
                Serial1.println(y);
            }
        }
    }

    Serial1.println("-------- END BOARD --------");
    Serial1.println();
}
