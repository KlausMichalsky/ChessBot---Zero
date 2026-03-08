#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/config.h"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : config.h
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definición de pines fisicos, constantes y tipos de datos.
//      - Definición de parametros globales.
//      - Definición de enums de estados.
//      - Definición estructuras de configuración de motores.
//      - Centralizar parámetros mecánicos dependientes del hardware.
// =======================================================================

#include <Arduino.h>

#pragma once

// PINES DE CONFIGURACIÓN
// -----------------------------------------------------------------------
// Sensor1, Encoder1, Motor1
#define HALL_1 3
#define AS5600_1_SDA 4
#define AS5600_1_SCL 5
#define MOTOR1_ENABLE 6
#define MOTOR1_DIR 7
#define MOTOR1_STEP 8
// Sensor2, Encoder2, Motor2
#define HALL_2 15
#define AS5600_2_SDA 26
#define AS5600_2_SCL 27
#define MOTOR2_ENABLE 12
#define MOTOR2_DIR 13
#define MOTOR2_STEP 14
// Sensor3, Encoder3, Motor3
#define HALL_3 29
#define MOTOR3_ENABLE 9
#define MOTOR3_DIR 10
#define MOTOR3_STEP 11
// Pines de LEDs y electroimán
#define LED 2
#define IMAN 28

// PARAMETROS DE CONFIGURACIÓN
// -----------------------------------------------------------------------
#define AS5600_ADDR 0x36
#define SEND_INTERVAL 33 // ms -> ~30Hz
#define DELTA_DEG 0.5f   // Enviar si el ángulo cambia más de DELTA_DEG

// NIVELES LÓGICOS DE ENABLE DEL TMC2209
// -----------------------------------------------------------------------
constexpr bool ENABLE_ACTIVE = LOW; // Nivel lógico (LOW=ON, HIGH=OFF)
constexpr bool ENABLE_INACTIVE = HIGH;

// TIPOS DE DATOS
// -----------------------------------------------------------------------
// Comandos recibidos por UART
enum class Command
{
    STATUS,
    RESET,
    HOME_MOTOR1,
    HOME_MOTOR2,
    HOME_MOTOR3,
    HOME_ALL,
    GET_ANGLE1,
    GET_ANGLE1_START,
    GET_ANGLE2,
    GET_ANGLE2_START,
    GET_ANGLE_STOP,
    UNKNOWN
};

// Estados de homing, motor1-2 (CW = ClockWise, CCW = CounterClockWise)
enum class HomingStateXY
{
    INACTIVE,
    FIND_FIRST_EDGE_CW,
    FIND_SECOND_EDGE_CW,
    FIND_FIRST_EDGE_CCW,
    FIND_SECOND_EDGE_CCW,
    SEARCH_FAST_CW,
    SEARCH_FAST_CCW,
    REVERSE_EDGE_CW,
    REVERSE_EDGE_CCW,
    CALC_CENTER,
    MOVE_TO_CENTER,
    OK,
    ERROR
};

// Estados de homing, motor3
enum class HomingStateZ
{
    INACTIVE,
    FIND_EDGE_DOWNWARD,
    FIND_EDGE_UPWARD,
    MOVE_TO_REFERENCE,
    OK,
    ERROR
};

// ESTRUCTURAS DE CONFIGURACIÓN DE MOTORES
// -----------------------------------------------------------------------
struct HomingConfig
{
    int microstepping;
    int reduction;
    int stepsPerRevolution;

    float fastSpeed;
    float slowSpeed;
    float acceleration;

    long steps90Deg;
    long stepsLimit;
    unsigned long timeout;
    int enablePin;
};

// Configuracion de Homing para cada motor, con parámetros mecánicos específicos
inline const HomingConfig motor1Config = {
    .microstepping = 16,
    .reduction = 9,
    .stepsPerRevolution = 200,
    .fastSpeed = 1500.0,
    .slowSpeed = 800.0,
    .acceleration = 1000.0,
    .steps90Deg = motor1Config.microstepping * motor1Config.stepsPerRevolution / 4,
    .timeout = 15000,
    .enablePin = MOTOR1_ENABLE};

inline const HomingConfig motor2Config = {
    .microstepping = 16,
    .reduction = 6,
    .stepsPerRevolution = 200,
    .fastSpeed = 1000.0,
    .slowSpeed = 533.0,
    .acceleration = 1000.0,
    .steps90Deg = motor2Config.microstepping * motor2Config.stepsPerRevolution / 4,
    .timeout = 15000,
    .enablePin = MOTOR2_ENABLE};

inline const HomingConfig motor3Config = {
    .microstepping = 8,
    .reduction = 1,
    .stepsPerRevolution = 200,
    .fastSpeed = 4000.0,
    .slowSpeed = 2500.0,
    .acceleration = 1000.0,
    .stepsLimit = -100, // pasos máximos si arranca fuera del imán
    .timeout = 12000,
    .enablePin = MOTOR3_ENABLE};
