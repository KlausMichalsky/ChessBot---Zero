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
#include "sensors.h"
#include "calc.h"

extern HomingState homingMotor1;
extern HomingState homingMotor2;
extern bool continuousAngle_1;

// COMPROBACIÓN DE COMANDOS DISPONIBLES -----------------------------------
bool commandAvailable()
{
    return Serial1.available();
}

// LECTURA DE COMANDOS ----------------------------------------------------------
String receiveCommand()
{
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    return cmd;
}

// PROCESAMIENTO DE COMANDOS -----------------------------------------------
Command parseCommand(const String &cmd) // Función para mapear String a enum
{
    if (cmd == "STATUS")
        return CMD_STATUS;
    if (cmd == "RESET_ERRORS")
        return CMD_RESET_ERRORS;
    if (cmd == "HOME_MOTOR1")
        return CMD_HOME_MOTOR1;
    if (cmd == "HOME_MOTOR2")
        return CMD_HOME_MOTOR2;
    if (cmd == "GET_ANGLE_1")
        return CMD_GET_ANGLE_1;
    if (cmd == "GET_ANGLE_1_START")
        return CMD_GET_ANGLE_1_START;
    if (cmd == "GET_ANGLE_1_STOP")
        return CMD_GET_ANGLE_1_STOP;
    return CMD_UNKNOWN;
}

void processCommand(const String &cmd)
{
    String trimmedCmd = cmd;
    trimmedCmd.trim(); // elimina \r\n y espacios

    switch (parseCommand(trimmedCmd))
    {
    case CMD_STATUS:
        if ((homingMotor1.state != HOMING_INACTIVE) &&
            (homingMotor1.state != HOMING_OK) &&
            (homingMotor1.state != HOMING_ERROR))
        {
            Serial1.println("HOMING IN PROGRESS");
        }
        else if (homingMotor1.state == HOMING_OK)
        {
            Serial1.println("HOMING MOTOR1 OK");
        }
        else if (homingMotor1.state == HOMING_ERROR)
        {
            Serial1.println("HOMING MOTOR1 ERROR");
        }
        else
        {
            Serial1.println("IDLE");
        }
        break;

    case CMD_RESET_ERRORS:
        homingMotor1.fault = false;
        homingMotor2.fault = false;
        Serial1.println("ERRORS RESET");
        break;

    case CMD_HOME_MOTOR1:
        Serial1.println("HOMING MOTOR1 STARTED");
        homingXY_Start(motor1, motor1Config, homingMotor1, HALL_1);
        break;

    case CMD_HOME_MOTOR2:
        Serial1.println("HOMING MOTOR2 STARTED");
        homingXY_Start(motor2, motor2Config, homingMotor2, HALL_2);
        break;

    case CMD_GET_ANGLE_1:
        sendAngle_1();
        break;

    case CMD_GET_ANGLE_1_START:
        continuousAngle_1 = true; // Activar lectura continua
        break;

    case CMD_GET_ANGLE_1_STOP:
        continuousAngle_1 = false; // Desactivar lectura continua
        break;

    default:
        Serial1.println("UNKNOWN COMMAND");
        break;
    }
}
