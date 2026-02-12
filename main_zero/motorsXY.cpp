// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : motorsXY.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : 2025-12-04
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      Implementación del módulo de control de motores.
//      Contiene la lógica interna para inicializar, habilitar,
//      mover y detener los motores físicos del robot.
//
//  ▫️ RESPONSABILIDADES
//      - Crear las instancias de los motores AccelStepper.
//      - Gestionar el estado interno de habilitación.
//      - Configurar velocidad y aceleración.
//      - Ejecutar movimientos coordinados en X e Y.
//      - Proveer parada segura y control de emergencia.
// =======================================================================
//  ▫️ ESTADO:
//      ⚠️ EN DESARROLLO ⚠️
//      Funcionalidad básica implementada y en fase de pruebas funcionales.
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include "motorsXY.h"
#include "config.h"

// INSTANCIAS DE MOTORES
// =======================================================================
// Crea las instancias de los motores utilizando la librería AccelStepper.
AccelStepper motor1(AccelStepper::DRIVER, MOTOR1_STEP, MOTOR1_DIR);
AccelStepper motor2(AccelStepper::DRIVER, MOTOR2_STEP, MOTOR2_DIR);

// ESTADO INTERNO DE MOTORES
// =======================================================================
// Esta variable controla si los motores están habilitados
// (true) o deshabilitados (false).
//
// ▸ static: hace que la variable sea **local al archivo motor.cpp**,
//   es decir, **no es visible desde otros módulos**.
// ▸ Se inicializa en false para que al arrancar el sistema
//   los motores estén apagados por seguridad.
// ▸ Se actualiza en motorsXY_Enable() y motorsXY_Disable() para que
//   otras funciones del módulo sepan si se puede mover el motor.
static bool motorsEnabled = false;

// API PÚBLICA (Application Programming Interface)
// =======================================================================
//   Inicializa los motores del sistema y prepara el hardware
//   para operar de manera segura.
//   Funcionalidad:
//   - Configura los pines ENABLE de cada motor como salida.
//   - Deshabilita los motores al arrancar para evitar movimientos
//     inesperados (estado seguro).
//   - Establece parámetros básicos de cada motor:
//       • Velocidad máxima (fastSpeed)
//       • Aceleración (acceleration)
//   - Prepara los motores para recibir comandos de movimiento
//   ⚠️ Nota:
//   Esta función debe llamarse **antes de cualquier intento de
//   mover los motores**, preferentemente al inicio del setup().
void motorsXY_Init()
{
    pinMode(HALL_1, INPUT_PULLUP);
    pinMode(HALL_2, INPUT_PULLUP);

    // ‼️ Si se cambian los valores de inversion
    // adaptar también en config.h las señales ENABLE_ACTIVE/INACTIVE (niveles lógicos de enable)
    motor1.setPinsInverted(true, false, false); // (DIR, STEP, ENABLE) true = invertir señal. Aqui: LOW=ON HIGH=OFF
    motor2.setPinsInverted(true, false, false); // (DIR, STEP, ENABLE) true = invertir señal. Aqui: LOW=ON HIGH=OFF

    // Configura los pines ENABLE de cada motor como salida
    // Estos pines controlan si el motor recibe energía o no
    pinMode(motor1Config.enablePin, OUTPUT);
    pinMode(motor2Config.enablePin, OUTPUT);

    // Deshabilita los motores al iniciar
    // ‼️ Esto evita movimientos inesperados al encender el sistema
    motorsXY_Disable();

    // Configuración de parámetros básicos de cada motor
    // Velocidad máxima (fastSpeed) y aceleración (acceleration)
    // Esto asegura movimientos suaves y controlables
    motor1.setMaxSpeed(motor1Config.fastSpeed);
    motor1.setAcceleration(motor1Config.acceleration);

    motor2.setMaxSpeed(motor2Config.fastSpeed);
    motor2.setAcceleration(motor2Config.acceleration);
}

void motorsXY_Enable()
{
    // Habilita los motores activando los pines ENABLE
    digitalWrite(motor1Config.enablePin, ENABLE_ACTIVE);
    digitalWrite(motor2Config.enablePin, ENABLE_ACTIVE);

    // Actualiza el estado interno indicando que los motores
    // están habilitados y listos para recibir comandos de movimiento
    motorsEnabled = true;
}

void motorsXY_Disable()
{
    // ver motorsXY_Enable() similar
    digitalWrite(motor1Config.enablePin, ENABLE_INACTIVE);
    digitalWrite(motor2Config.enablePin, ENABLE_INACTIVE);
    motorsEnabled = false;
}

void motorsXY_Move(long x, long y)
{
    // ⚠️ Nota: No ejecuta el movimiento inmediatamente.
    //       La función motorRun() debe llamarse repetidamente
    //       en loop() para que los motores se desplacen
    //       hacia las posiciones indicadas.
    if (!motorsEnabled)
        motorsXY_Enable();
    motor1.moveTo(x);
    motor2.moveTo(y);
}

void motorsXY_SetSpeed(long speed)
{
    motor1.setMaxSpeed(speed);
    motor2.setMaxSpeed(speed);
}

void motorsXY_Run()
{
    if (!motorsEnabled)
        return; // 🔒 motor apagado = no ejecutar movimiento
    motor1.run();
    motor2.run();
}

void motorsXY_Stop()
{
    motor1.stop();
    motor2.stop();
    motorsXY_Disable(); // 🔥 CLAVE: apaga físicamente
}

bool motorsXY_Done()
{
    // Devuelve true si **ambos motores han llegado** a su posición objetivo
    // ▸ distanceToGo() devuelve la distancia restante que le falta
    //    al motor para alcanzar el objetivo definido por moveTo().
    // ▸ Se verifica que motor1 y motor2 hayan terminado su movimiento.
    // ▸ Esto permite que otras funciones (por ejemplo, homing o secuencias
    //    de movimiento) sepan cuándo se completó el desplazamiento.
    return motor1.distanceToGo() == 0 &&
           motor2.distanceToGo() == 0;
}
