#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/commands.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================

//  Archivo    : commands.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones para la gestión de comandos
//        recibidos por UART usando enum class Command
// =======================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h" // enum class Command
#include "commands.h"
#include "homingXY.h"
#include "homingZ.h"
#include "motorsXY.h"
#include "sensors.h"
#include "calc.h"

// Variables externas
extern HomingRunTimeXY homingMotor1;
extern HomingRunTimeXY homingMotor2;
extern HomingRunTimeZ homingMotor3;
extern bool dynamicAngle1;
extern bool dynamicAngle2;

// -----------------------------------------------------------------------
// COMPROBACIÓN DE COMANDOS DISPONIBLES
// -----------------------------------------------------------------------
bool commandAvailable()
{
    return Serial1.available();
}

// -----------------------------------------------------------------------
// LECTURA DE COMANDOS
// -----------------------------------------------------------------------
String receiveCommand()
{
    static String buffer = "";
    while (Serial1.available())
    {
        char c = Serial1.read();
        if (c == '\n')
        {
            String cmd = buffer;
            buffer = "";
            cmd.trim();
            return cmd;
        }
        else
        {
            buffer += c;
        }
    }
    return ""; // no hay comando completo todavía
}

// -----------------------------------------------------------------------
// MAPEAR STRING A enum class Command
// -----------------------------------------------------------------------
Command parseCommand(const String &cmd)
{
    if (cmd == "STATUS")
        return Command::STATUS;
    if (cmd == "RESET-ERRORS")
        return Command::RESET_ERRORS;
    if (cmd == "HOME-MOTOR1")
        return Command::HOME_MOTOR1;
    if (cmd == "HOME-MOTOR2")
        return Command::HOME_MOTOR2;
    if (cmd == "HOME-MOTOR3")
        return Command::HOME_MOTOR3;
    if (cmd == "GET-ANGLE1")
        return Command::GET_ANGLE1;
    if (cmd == "GET-ANGLE1-START")
        return Command::GET_ANGLE1_START;
    if (cmd == "GET-ANGLE2")
        return Command::GET_ANGLE2;
    if (cmd == "GET-ANGLE2-START")
        return Command::GET_ANGLE2_START;
    if (cmd == "GET-ANGLE-STOP")
        return Command::GET_ANGLE_STOP;
    return Command::UNKNOWN;
}

// -----------------------------------------------------------------------
// ENVIAR RESPUESTA POR UART
// -----------------------------------------------------------------------
void sendResponse(const String &msg)
{
    while (Serial1.available())
        Serial1.read(); // limpiar residuos
    Serial1.println(msg);
}

// -----------------------------------------------------------------------
// PROCESAMIENTO DE COMANDOS
// -----------------------------------------------------------------------
void processCommand(const String &cmdStr)
{
    String trimmedCmd = cmdStr;
    trimmedCmd.trim();

    Command cmd = parseCommand(trimmedCmd);

    switch (cmd)
    {
    case Command::STATUS:
        if ((homingMotor1.state != HomingStateXY::INACTIVE) &&
            (homingMotor1.state != HomingStateXY::OK) &&
            (homingMotor1.state != HomingStateXY::ERROR))
        {
            Serial1.println("HOMING IN PROGRESS");
        }
        else if (homingMotor1.state == HomingStateXY::OK)
        {
            Serial1.println("HOMING MOTOR1 OK");
        }
        else if (homingMotor1.state == HomingStateXY::ERROR)
        {
            Serial1.println("HOMING MOTOR1 ERROR");
        }
        else
        {
            sendResponse("IDLE");
        }
        break;

    case Command::RESET_ERRORS:
        homingMotor1.fault = false;
        homingMotor2.fault = false;
        sendResponse("ERRORS RESET");
        break;

    case Command::HOME_MOTOR1:
        sendResponse("HOMING MOTOR1 STARTED");
        homingXY_Start(motor1, motor1Config, homingMotor1, HALL_1);
        break;

    case Command::HOME_MOTOR2:
        sendResponse("HOMING MOTOR2 STARTED");
        homingXY_Start(motor2, motor2Config, homingMotor2, HALL_2);
        break;

    case Command::HOME_MOTOR3:
        sendResponse("HOMING MOTOR3 STARTED");
        homingZ_Start(motor3, motor3Config, homingMotor3, HALL_3);
        break;

    case Command::GET_ANGLE1:
        Serial1.print("ANGLE1: ");
        sendStaticAngle(Wire); // Leer y enviar ángulo del primer sensor
        break;

    case Command::GET_ANGLE1_START:
        dynamicAngle1 = true; // Activar lectura continua
        break;

    case Command::GET_ANGLE2:
        Serial1.print("ANGLE2: ");
        sendStaticAngle(Wire1); // Leer y enviar ángulo del segundo sensor
        break;

    case Command::GET_ANGLE2_START:
        dynamicAngle2 = true; // Activar lectura continua
        break;

    case Command::GET_ANGLE_STOP:
        dynamicAngle1 = false;
        dynamicAngle2 = false;
        break;

    case Command::UNKNOWN:
    default:
        sendResponse("UNKNOWN COMMAND");
        break;
    }
}