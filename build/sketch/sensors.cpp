#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\sensors.cpp"
// =======================================================================
//                    🔹 P I C O   —   C H E S S 🔹
// =======================================================================
//  Archivo    : sensores.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Implementación de funciones para la lectura y gestión de
//      sensores del robot de ajedrez.
//  ▫️ RESPONSABILIDADES:
//      - Inicializar y configurar sensores.
//      - Leer valores de sensores de posición o proximidad.
//      - Validar y filtrar datos obtenidos.
//      - Proveer funciones de soporte para otros módulos.
// =======================================================================

// sensores.cpp
#include <Arduino.h>
#include "sensors.h"
#include "config.h"

void inicializarSensores()
{
    // Wire1.begin(AS5600_1_SDA, AS5600_1_SCL);
    // Wire2.begin(AS5600_2_SDA, AS5600_2_SCL);
}

float leerAS5600_1() { return 0.0; }
float leerAS5600_2() { return 0.0; }
bool leerHall_1() { return digitalRead(HALL_1); }
bool leerHall_2() { return digitalRead(HALL_2); }
