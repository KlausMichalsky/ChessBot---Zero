// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : commands.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones para la gestión de comandos
//        recibidos por UART
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"
#include "commands.h"
#include "homingXY.h"
#include "motorsXY.h"

extern HomingState homingMotor1;
extern HomingState homingMotor2;

// COMPROBACIÓN DE COMANDOS DISPONIBLES
// =======================================================================
bool commandAvailable()
{
    return Serial1.available();
}

// LEER COMANDOS
// =======================================================================
String receiveCommand()
{
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    return cmd;
}

// PROCESAMIENTO DE COMANDOS
// =======================================================================
void processCommand(const String &cmd)
{
    if (cmd == "STATUS")
    {
        if ((homingMotor1.state != HOMING_INACTIVE) && (homingMotor1.state != HOMING_OK) && (homingMotor1.state != HOMING_ERROR))
        {
            Serial1.println("HOMING IN PROGRESS");
        }
        else
        {
            Serial1.println("IDLE");
        }
    }
    else if (cmd == "RESET_ERRORS")
    {
        homingMotor1.fault = false;
        homingMotor2.fault = false;
        Serial1.println("ERRORS RESET");
    }
    else if (cmd == "HOME_MOTOR1")
    {
        Serial1.println("HOMING MOTOR1 STARTED");
        homingXY_Start(motor1, motor1Config, homingMotor1, HALL_1);
    }
    else if (cmd == "HOME_MOTOR2")
    {
        Serial1.println("HOMING MOTOR2 STARTED");
        homingXY_Start(motor2, motor2Config, homingMotor2, HALL_2);
    }
    else
    {
        Serial1.println("UNKNOWN COMMAND");
    }
}
