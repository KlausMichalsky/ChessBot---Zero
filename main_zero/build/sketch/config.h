#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/main_zero/config.h"
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

#define LED 2
#define BOTON 28 // cambiar a 28 para Zero A0 para Nano (solo usado en la fase de pruebas)

// NIVELES LÓGICOS DE ENABLE
// =======================================================================
// Define los niveles lógicos para habilitar y deshabilitar los motores
// Si se invierte alguna señal en setup(), cambiar aquí también
// constexpr: indica que es una constante en tiempo de compilación
// -> no existe como variable en tiempo de ejecución
// -> el compilador reemplaza ENABLE_ACTIVE por LOW directamente en el código
constexpr bool ENABLE_ACTIVE = LOW;    // Nivel lógico para habilitar motor (LOW=ON, HIGH=OFF)
constexpr bool ENABLE_INACTIVE = HIGH; // Nivel lógico para deshabilitar motor

// TIPOS Y ESTRUCTURAS
// =======================================================================
// Tipo enumerado (enum)
// Posibles estados de la rutina de homing (CW = ClockWise, CCW = CounterClockWise)
// Estructura que agrupa la configuración de los motores para la rutina de homing
// Se utiliza para almacenar todos los parámetros necesarios para cada motor
enum EstadoHoming
{
    HOMING_INACTIVE,             // 🔹 Homing apagado / no activo
    HOMING_FIND_FIRST_EDGE_CW,   // 🔹 Buscar el primer flanco del imán en sentido horario
    HOMING_FIND_SECOND_EDGE_CW,  // 🔹 Buscar el segundo flanco del imán en sentido horario
    HOMING_FIND_FIRST_EDGE_CCW,  // 🔹 Buscar el primer flanco del imán en sentido antihorario
    HOMING_FIND_SECOND_EDGE_CCW, // 🔹 Buscar el segundo flanco del imán en sentido antihorario
    HOMING_SEARCH_FAST_CW,       // 🔹 Movimiento rápido inicial en sentido horario hasta detectar el imán
    HOMING_SEARCH_FAST_CCW,      // 🔹 Movimiento rápido inicial en sentido antihorario hasta detectar el imán
    HOMING_REVERSE_EDGE_CW,      // 🔹 Invertir dirección tras primer flanco para encontrar el segundo (horario)
    HOMING_REVERSE_EDGE_CCW,     // 🔹 Invertir dirección tras primer flanco para encontrar el segundo (antihorario)
    HOMING_CALC_CENTER,          // 🔹 Calcular el centro entre los flancos detectados
    HOMING_MOVE_TO_CENTER,       // 🔹 Mover motor hacia el centro calculado (referencia)
    HOMING_OK,                   // 🔹 Homing completado correctamente
    HOMING_ERROR                 // 🔹 Ocurrió un error en homing (timeout, sensor no detectado, límite alcanzado)
};

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

// CONFIGURACIONES DE MOTORES
// =======================================================================
// Incluye todos los parámetros mecánicos y de velocidad necesarios para cada motor
// El punto . delante de cada nombre de campo dentro de la inicialización
// de la estructura se llama “designated initializer” o inicializador designado.
// Significa que le estás diciendo explícitamente a qué campo de la estructura
// va cada valor, sin importar el orden.
const HomingConfig motor1Config = {
    .microstepping = 16,
    .reduction = 1,
    .stepsPerRevolution = 200,
    .fastSpeed = 1500.0,
    .slowSpeed = 750.0,
    .acceleration = 1000.0,
    .steps90Deg = 16 * 1 * 200 / 4,
    .timeout = 15000,
    .enablePin = MOTOR1_ENABLE};

const HomingConfig motor2Config = {
    .microstepping = 16,
    .reduction = 1,
    .stepsPerRevolution = 200,
    .fastSpeed = 400.0,
    .slowSpeed = 200.0,
    .acceleration = 800.0,
    .steps90Deg = 16 * 1 * 200 / 4,
    .timeout = 15000,
    .enablePin = MOTOR2_ENABLE};

// Aqui agregar más configuraciones de motores si es necesario