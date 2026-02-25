// =======================================================================
//                    🔹 P I C O   —   C H E S S 🔹
// =======================================================================
//  Archivo    : sensores.h
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Declaración de funciones y constantes para el manejo
//      de sensores del robot de ajedrez.
//  ▫️ RESPONSABILIDADES:
//      - Proveer prototipos de funciones de inicialización y lectura.
//      - Definir constantes y macros para sensores.
//      - Servir de interfaz clara para el resto del proyecto.
// =======================================================================

#pragma once
#include <Wire.h>

// Declaraciones externas de las variables globales
// 1️⃣ Qué hace extern
// extern le dice al compilador:
// “Esta variable existe en otro lugar, no la declares de nuevo aquí, solo quiero usarla.”
// Es una declaración, no una definición.
// Declaración (extern) → solo le dice al compilador “la variable existe en algún lado, confía en mí”.
// Definición → realmente reserva memoria para la variable.
// 2️⃣ Por qué se pone en el .h
// El .h se incluye en varios archivos (.ino, otros .cpp).
// Si ponemos extern en el .h, todos los archivos que incluyan ese .h sabrán que la variable existe pero no crearán una copia.
// Esto evita errores de “variable ya definida” (multiple definition).
extern float lastSentAngle_1;
extern unsigned long lastSendTime_1;

extern float lastSentAngle_2;
extern unsigned long lastSendTime_2;

void sensors_Init();
uint16_t readAngle(TwoWire &wire);
void sendDynamicAngle(TwoWire &wire, float &lastSentAngle, unsigned long &lastSendTime);
void sendStaticAngle(TwoWire &wire);
