// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : main_zero.ino
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Control principal del robot físico.
//      - Gestiona motores, sensores,
//        LEDs y la comunicación con el cerebro lógico del Pico.
//      - Inicializar y controlar motores y actuadores.
//      - Leer y procesar datos de sensores.
//      - Controlar los LEDs indicadores.
//      - Gestionar la comunicación con el Pico.
//      - Ejecutar los movimientos físicos del robot de ajedrez.
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <Bounce2.h>
#include "communication.h"
#include "commands.h"
#include "config.h"
#include "filtro.h"
#include "motorsXY.h"
#include "sensors.h"
#include "homingXY.h"

// DEFINICION DE OBJETOS
// -----------------------------------------------------------------------
HomingState homingMotor1;
HomingState homingMotor2;
Bounce debouncer; // Crea un objeto para el botón (solo en la fase de pruebas)

// FLAGS Y VARIABLES GLOBALES
// -----------------------------------------------------------------------
bool continuousAngle_1 = false; // Flag para lectura continua del ángulo AS5600

// SETUP
// -----------------------------------------------------------------------
void setup()
{
    Serial.begin(115200);

    // Electroiman
    pinMode(IMAN, INPUT_PULLUP);

    // LED indicador
    pinMode(LED, OUTPUT);

    // Inicialización de comunicación UART
    UART_Init();

    // Inicialización de motores y homing
    motorsXY_Init();
    homingXY_Init(homingMotor1);
    homingXY_Init(homingMotor2);
}

// LOOP
// -----------------------------------------------------------------------
void loop()
{
    if (commandAvailable())
    {
        String cmd = receiveCommand();
        debug(cmd); // solo imprime el comando recibido si DEBUG_UART = 1
        processCommand(cmd);
    }
    updateTasks();
}

void updateTasks()
{
    // Homing de motores (sin bloquear el loop)
    homingXY_Step(motor1, motor1Config, homingMotor1, HALL_1);
    homingXY_Step(motor2, motor2Config, homingMotor2, HALL_2);

    // Lectura continua del ángulo 1 si el flag está activo
    if (continuousAngle_1)
    {
        sendAngle_1();
    }

    // Aqui se podrían agregar otras tareas periódicas,
    // como actualizar el estado de otros motores, leer sensores,
    // controlar LEDs, etc.
}
