#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\config.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : communication.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definir pines de motores, sensores y LEDs.
//      - Declarar enums de estados.
//      - Definir estructuras de configuración de motores.
//      - Centralizar parámetros mecánicos dependientes del hardware.
/*
| Qué poner en `config.h`                                 |
| ------------------------------------------------------- |
| Constantes, direcciones, límites, parámetros ajustables |

| Qué poner en `.cpp` / `.ino`                           |
| ------------------------------------------------------ |
| Variables de estado, contadores, temporizadores, flags |
*/
// =======================================================================

#pragma once
#include <Arduino.h>

// PINES DE CONFIGURACIÓN
// =======================================================================
// Sensor1, Encoder1, Motor 1
#define HALL_1 3
#define AS5600_1_SDA 4
#define AS5600_1_SCL 5
#define MOTOR1_ENABLE 6
#define MOTOR1_DIR 7
#define MOTOR1_STEP 8
// Sensor2, Encoder2, Motor 2
#define HALL_2 15
#define AS5600_2_SDA 26
#define AS5600_2_SCL 27
#define MOTOR2_ENABLE 12
#define MOTOR2_DIR 13
#define MOTOR2_STEP 14
// Sensor3, Encoder3, Motor 3
#define HALL_3 29
#define MOTOR3_ENABLE 9
#define MOTOR3_DIR 10
#define MOTOR3_STEP 11

// Pines de LEDs y electroimán
#define LED 2
#define IMAN 28

// PARAMETROS DE CONFIGURACIÓN
// =======================================================================
// Dirección I2C del sensor AS5600
// (direccion común pero perifericos I2C distintos)
// -> Wire para AS5600_1, Wire1 para AS5600_2)
#define AS5600_ADDR 0x36

// Config. de transmisión de angulo por UART
#define SEND_INTERVAL 33 // ms -> ~30Hz
#define DELTA_DEG 0.5f   // Enviar solo si el ángulo cambia más de este valor

// NIVELES LÓGICOS DE ENABLE
// =======================================================================
// Define los niveles lógicos para habilitar y deshabilitar los motores
// Si se invierte alguna señal en setup(), cambiar aquí también
// constexpr: indica que es una constante en tiempo de compilación
// -> no existe como variable en tiempo de ejecución
// -> el compilador reemplaza ENABLE_ACTIVE por LOW directamente en el código
constexpr bool ENABLE_ACTIVE = LOW; // Nivel lógico (LOW=ON, HIGH=OFF)
constexpr bool ENABLE_INACTIVE = HIGH;

// TIPOS DE DATOS Y ENUMERADOS
// =======================================================================
// Tipo enumerado (enum) para los comandos recibidos por UART
enum class Command
{
    STATUS,
    RESET_ERRORS,
    HOME_MOTOR1,
    HOME_MOTOR2,
    GET_ANGLE1,
    GET_ANGLE1_START,
    GET_ANGLE2,
    GET_ANGLE2_START,
    GET_ANGLE_STOP,
    UNKNOWN
};

// Tipo enumerado (enum) para los estados de la rutina de homing (CW = ClockWise, CCW = CounterClockWise)
enum class HomingStateXY
{
    INACTIVE,             // 🔹 Homing apagado / no activo
    FIND_FIRST_EDGE_CW,   // 🔹 Buscar el primer flanco del imán en sentido horario
    FIND_SECOND_EDGE_CW,  // 🔹 Buscar el segundo flanco del imán en sentido horario
    FIND_FIRST_EDGE_CCW,  // 🔹 Buscar el primer flanco del imán en sentido antihorario
    FIND_SECOND_EDGE_CCW, // 🔹 Buscar el segundo flanco del imán en sentido antihorario
    SEARCH_FAST_CW,       // 🔹 Movimiento rápido inicial en sentido horario hasta detectar el imán
    SEARCH_FAST_CCW,      // 🔹 Movimiento rápido inicial en sentido antihorario hasta detectar el imán
    REVERSE_EDGE_CW,      // 🔹 Invertir dirección tras primer flanco para encontrar el segundo (horario)
    REVERSE_EDGE_CCW,     // 🔹 Invertir dirección tras primer flanco para encontrar el segundo (antihorario)
    CALC_CENTER,          // 🔹 Calcular el centro entre los flancos detectados
    MOVE_TO_CENTER,       // 🔹 Mover motor hacia el centro calculado (referencia)
    OK,                   // 🔹 Homing completado correctamente
    ERROR                 // 🔹 Ocurrió un error en homing (timeout, sensor no detectado, límite alcanzado)

};

// ESTRUCTURAS DE CONFIGURACIÓN
// =======================================================================
struct HomingConfig
{
    int microstepping;
    int reduction;
    int stepsPerRevolution;

    float fastSpeed;
    float slowSpeed;
    float acceleration;

    long steps90Deg;
    unsigned long timeout;
    int enablePin;
};

const HomingConfig motor1Config = {
    .microstepping = 16,
    .reduction = 9,
    .stepsPerRevolution = 200,
    .fastSpeed = 1500.0,
    .slowSpeed = 800.0,
    .acceleration = 1000.0,
    .steps90Deg = motor1Config.microstepping * motor1Config.stepsPerRevolution / 4,
    .timeout = 15000,
    .enablePin = MOTOR1_ENABLE};

const HomingConfig motor2Config = {
    .microstepping = 16,
    .reduction = 6,
    .stepsPerRevolution = 200,
    .fastSpeed = 1000.0,
    .slowSpeed = 533.0,
    .acceleration = 1000.0,
    .steps90Deg = motor2Config.microstepping * motor2Config.stepsPerRevolution / 4,
    .timeout = 15000,
    .enablePin = MOTOR2_ENABLE};

// Aqui agregar más configuraciones de motores si es necesario