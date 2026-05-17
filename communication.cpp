// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : communication.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Inicializar UART
//      - Comunicación UART con el Raspberry
//      - Gestionar la recepción de datos de forma no bloqueante.
// =======================================================================

#include <Arduino.h>

#include "communication.h"
#include "config.h"

// CONFIGURACIÓN DE DEPURACIÓN UART
// -----------------------------------------------------------------------
void debug(const String &msg) {
#if DEBUG_UART
    Serial1.println("Comando recibido: " + msg);
#endif
}

// INICIALIZACIÓN DE COMUNICACION UART
// -----------------------------------------------------------------------
void communicationInit() {
    // USB para debug (opcional)
    Serial.begin(115200);

    // UART hardware en pines 0 y 1
    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);
    while (Serial1.available())
        Serial1.read(); // limpia buffer UART
}
