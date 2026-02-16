#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\ChessBot---Zero.ino"
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
#include "led.h"
#include "motorsXY.h"
#include "sensors.h"
#include "homingXY.h"

// DEFINICION DE OBJETOS
// -----------------------------------------------------------------------
HomingState homingMotor1;
HomingState homingMotor2;
Bounce debouncer; // Crea un objeto para el botón (solo en la fase de pruebas)

// SETUP
// -----------------------------------------------------------------------
#line 41 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\ChessBot---Zero.ino"
void setup();
#line 63 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\ChessBot---Zero.ino"
void loop();
#line 74 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\ChessBot---Zero.ino"
void updateTasks();
#line 41 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\ChessBot---Zero.ino"
void setup()
{
    Serial.begin(115200);
    // Boton con debounce
    pinMode(BOTON, INPUT_PULLUP);
    debouncer.attach(BOTON);
    debouncer.interval(25); // 25 ms de debounce

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
    homingXY_Step(motor1, motor1Config, homingMotor1, HALL_1);
    homingXY_Step(motor2, motor2Config, homingMotor2, HALL_2);

    // Aqui se podrían agregar otras tareas periódicas,
    // como actualizar el estado de otros motores, leer sensores,
    // controlar LEDs, etc.
}

