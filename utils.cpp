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

    // VALIDAR FILA Y RANGO
    if (file < 'A' || file > 'H') {
        return false;
    }
    if (rank < '1' || rank > '8') {
        return false;
    }

    // CONVERTIR A ÍNDICES
    int fileIndex = file - 'A';
    int rankIndex = rank - '1';

    // CONVERTIR A COORDENADAS EN CENTRO DE CASILLAS
    x = A1_OFFSET_X + (fileIndex * SQUARE_SIZE);
    y = A1_OFFSET_Y + (rankIndex * SQUARE_SIZE);

    return true;
}

// // CONVERSIÓN XY → ANGULOS (CINEMATICA INVERSA)
// // -----------------------------------------------------------------------
// // “devuelve” theta1 y theta2 porque -> & modifica variables mediante referencias
// // La matemática trigonométrica de C++ usa: sin, cos, atan2, acos TODO en radianes
// // Ejemplo PI rad = 180°
// bool xyToIKRadians(
//     float x,
//     float y,
//     float L1,
//     float L2,
//     float &theta1, // El & significa NO crea copias. Modifica directamente las variables originales
//     float &theta2) {
//     float r2 = x * x + y * y;
//     float r = sqrt(r2);

//     // CHECK: punto alcanzable
//     if (r > (L1 + L2) || r < fabs(L1 - L2)) {
//         return false; // fuera del alcance
//     }

//     // Ley de cosenos para calcular del ángulo del codo.
//     float cosTheta2 = (r2 - L1 * L1 - L2 * L2) / (2 * L1 * L2);
//     theta2 = acos(cosTheta2); // queda en radianes

//     // Parte auxiliar para calcular el hombro.
//     float k1 = L1 + L2 * cos(theta2); // Representa la proyección horizontal.
//     float k2 = L2 * sin(theta2);      // Representa la proyección vertical.

//     // theta1 = apunta hacia el objetivo - corrige el ángulo porque existe el segundo brazo
//     theta1 = atan2(y, x) - atan2(k2, k1);

//     return true;
// }

// // CONVERSION DE RADIANES A GRADOS
// // -----------------------------------------------------------------------
// float radToDegrees(float rad) {
//     return rad * 180.0 / PI;
// }

// // CONVERSION DE GRADOS A RADIANES
// // -----------------------------------------------------------------------
// float degreesToRad(float deg) {
//     return deg * PI / 180.0;
// }

// CONVERSION DE CASSILA A ANGULOS
// -----------------------------------------------------------------------
// 1️⃣ Casilla → XY
// "E4"
// ↓
// (12, 220)
// 2️⃣ XY → IK
// (12,220)
// ↓
// (1.2 rad, 0.7 rad)
// 3️⃣ Radianes → grados
// (68°, 40°)
// 4️⃣ Devuelve:
// theta1Deg
// theta2Deg
// bool chessSquareToAngles(
//     const String &square,
//     float &theta1Deg,
//     float &theta2Deg) {
//     float x;
//     float y;

//     // CASILLA -> XY
//     if (!chessSquareToXY(square, x, y)) { // si casilla invalida -> salir
//         return false;
//     }

//     float theta1Rad;
//     float theta2Rad;

//     // XY -> IK
//     if (!xyToIKRadians( // si punto fuera de alcance o geometría imposible -> salir
//             x,
//             y,
//             LINK1,
//             LINK2,
//             theta1Rad,
//             theta2Rad)) {
//         return false;
//     }

//     // RAD -> DEG
//     theta1Deg = radToDegrees(theta1Rad);
//     theta2Deg = radToDegrees(theta2Rad);

//     return true;
// }

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
