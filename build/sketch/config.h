#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\config.h"
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

#pragma once

#include <Arduino.h>

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
#define MAGNET 28

// PARAMETROS DE CONFIGURACIÓN PLANO XY
// -----------------------------------------------------------------------
#define AS5600_ADDR 0x36
#define SEND_INTERVAL 33    // ms -> ~30Hz
#define DELTA_DEG 0.5f      // Enviar si el ángulo cambia más de DELTA_DEG
#define BASE_SPEED 2 * 1500 // usada para igualar tiempo de llegada de motor1 y 2
// BASE_SPEED: velocidad máxima que tendrá el motor que recorre la mayor distancia
// ✔ suficientemente alta para que el movimiento no sea lento
// ✔ suficientemente baja para que ningún motor pierda pasos

// PARAMETROS DE CONFIGURACIÓN EJE Z
// -----------------------------------------------------------------------
#define Z_STEPS_DOWN 12600 // cantidad de pasos para bajar
#define Z_DELAY 100        // delay entre movimientos para darle tiempo al iman

// NIVELES LÓGICOS DE ENABLE DEL TMC2209
// -----------------------------------------------------------------------
constexpr bool ENABLE_ACTIVE = LOW; // Nivel lógico (LOW=ON, HIGH=OFF)
constexpr bool ENABLE_INACTIVE = HIGH;

// TIPOS DE DATOS
// -----------------------------------------------------------------------
// Comandos recibidos por UART
enum class Command {
    STATUS,
    RESET,
    HOME1,
    HOME2,
    HOME3,
    HOME,
    ANGLES,
    PICK,
    PLACE,
    MOVE,
    SHOW_COMMANDS,
    UNKNOWN
};

// Estados de homing, motor1-2 (CW = ClockWise, CCW = CounterClockWise)
enum class HomingStateXY {
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
enum class HomingStateZ {
    INACTIVE,
    FIND_EDGE_DOWNWARD,
    FIND_EDGE_UPWARD,
    MOVE_TO_REFERENCE,
    OK,
    ERROR
};

// Máquina de estado para HOME_ALL
enum class HomeAllState {
    IDLE,
    MOTOR1,
    MOTOR2,
    MOTOR3,
    DONE
};

// Maquina de estado para Motores individuales
enum class HomeSingleState {
    IDLE,
    RUNNING,
    DONE
};

enum class MotorID {
    NONE,
    J1,
    J2,
    Z
};
// ESTRUCTURAS DE CONFIGURACIÓN DE MOTORES
// -----------------------------------------------------------------------
struct MotorConfig {
    // Mecanica
    int microstepping;      // Microstepping del driver
    int reduction;          // Relación de reducción mecánica
    int stepsPerRevolution; // Pasos por revolución del motor

    // Homing
    float slowSpeed;       // Velocidad lenta durante homing
    float fastSpeed;       // Velocidad rápida durante homing
    long steps90Deg;       // Pasos equivalentes a 90°
    long stepsLimit;       // Límite máximo de pasos
    unsigned long timeout; // Tiempo máximo permitido en homing

    // Movimiento a coordenadas
    float baseSpeed;    // Velocidad máxima de referencia
    float acceleration; // Aceleración máxima

    // Pines
    int enablePin;
};

// Configuracion de Homing para cada motor, con parámetros mecánicos específicos
inline const MotorConfig motor1Config = {
    .microstepping = 16,
    .reduction = 9,
    .stepsPerRevolution = 200,
    .slowSpeed = 2 * 800.0,
    .fastSpeed = 2 * 1500.0,
    .steps90Deg = motor1Config.microstepping * motor1Config.stepsPerRevolution / 4,
    .stepsLimit = 0, // no existe para motor1
    .timeout = 15000,
    .baseSpeed = BASE_SPEED,
    .acceleration = 2 * 1000.0,
    .enablePin = MOTOR1_ENABLE};

inline const MotorConfig motor2Config = {
    .microstepping = 16,
    .reduction = 6,
    .stepsPerRevolution = 200,
    .slowSpeed = 2 * 533.0,
    .fastSpeed = 2 * 1000.0,
    .steps90Deg = motor2Config.microstepping * motor2Config.stepsPerRevolution / 4,
    .stepsLimit = 0, // no existe para motor2
    .timeout = 15000,
    .baseSpeed = BASE_SPEED,
    .acceleration = 2 * 1000.0,
    .enablePin = MOTOR2_ENABLE};

inline const MotorConfig motor3Config = {
    .microstepping = 8,
    .reduction = 1,
    .stepsPerRevolution = 200,
    .slowSpeed = 2500.0,
    .fastSpeed = 4000.0,
    .steps90Deg = 0,    // no existe para motor3
    .stepsLimit = -100, // pasos máximos si arranca fuera del imán (solo motor3)
    .timeout = 12000,
    .baseSpeed = BASE_SPEED,
    .acceleration = 1000.0,
    .enablePin = MOTOR3_ENABLE};
