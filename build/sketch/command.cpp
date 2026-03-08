#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/command.cpp"
// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : command.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Implementación de funciones para la gestión de comandos
//        recibidos por UART
// =======================================================================

#include <Arduino.h>

#include <AccelStepper.h>

#include "command.h"
#include "config.h"
#include "core.h"
#include "homing.h"
#include "motors.h"
#include "sensors.h"
#include "utils.h"

// VARIABLES EXTERNAS
// -----------------------------------------------------------------------
extern HomingXY motor1Homing;
extern HomingXY motor2Homing;
extern HomingZ motor3Homing;
extern bool dynamicAngle1;
extern bool dynamicAngle2;
extern bool homeAllActive;

// COMPROBACIÓN DE COMANDOS DISPONIBLES
// -----------------------------------------------------------------------
bool commandAvailable()
{
    return Serial1.available();
}

// GENERAR RESPUESTA DE STATUS DE TODOS LOS MOTORES
// -----------------------------------------------------------------------
String commandStatusReport()
{
    String resp = "";

    // Motor1
    switch (motor1Homing.state)
    {
    case HomingStateXY::OK:
        resp += "MOTOR1 OK; ";
        break;
    case HomingStateXY::ERROR:
        resp += "MOTOR1 ERROR; ";
        break;
    case HomingStateXY::INACTIVE:
        break; // nada que mostrar
    default:
        resp += "MOTOR1 RUNNING; ";
        break;
    }

    // Motor2
    switch (motor2Homing.state)
    {
    case HomingStateXY::OK:
        resp += "MOTOR2 OK; ";
        break;
    case HomingStateXY::ERROR:
        resp += "MOTOR2 ERROR; ";
        break;
    case HomingStateXY::INACTIVE:
        break;
    default:
        resp += "MOTOR2 RUNNING; ";
        break;
    }

    // Motor3
    switch (motor3Homing.state)
    {
    case HomingStateZ::OK:
        resp += "MOTOR3 OK; ";
        break;
    case HomingStateZ::ERROR:
        resp += "MOTOR3 ERROR; ";
        break;
    case HomingStateZ::INACTIVE:
        break;
    default:
        resp += "MOTOR3 RUNNING; ";
        break;
    }

    if (resp == "")
        resp = "IDLE"; // si ningún motor tiene actividad

    return resp;
}

// LECTURA DE COMANDOS
// -----------------------------------------------------------------------
String readCommand()
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

// MAPEAR STRING a enum class Command
// -----------------------------------------------------------------------
Command parseCommand(const String &cmd)
{
    if (cmd == "STATUS")
        return Command::STATUS;
    if (cmd == "RESET")
        return Command::RESET;
    if (cmd == "HOME-MOTOR1")
        return Command::HOME_MOTOR1;
    if (cmd == "HOME-MOTOR2")
        return Command::HOME_MOTOR2;
    if (cmd == "HOME-MOTOR3")
        return Command::HOME_MOTOR3;
    if (cmd == "HOME-ALL")
        return Command::HOME_ALL;
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

// ENVIAR RESPUESTA POR UART
// -----------------------------------------------------------------------
void commandSendResponse(const String &msg)
{
    // while (Serial1.available())              ⚠️
    //     Serial1.read(); // limpiar residuos  ⚠️
    Serial1.println(msg);
}

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
        commandSendResponse(commandStatusReport());
        break;

    case Command::RESET:
        motor1Homing.fault = false;
        motor2Homing.fault = false;
        motor3Homing.fault = false;        // si querés limpiar Z también
        homingInitXY(motor1Homing);        // reinicia motor1
        homingInitXY(motor2Homing);        // reinicia motor2
        homingInitZ(motor3Homing);         // reinicia motor3
        homeAllState = HomeAllState::IDLE; // si estabas en HOME-ALL, cancelalo
        commandSendResponse("RESET");
        break;

    case Command::HOME_MOTOR1:
        commandSendResponse("HOMING MOTOR1 STARTED");
        homingStartXY(motor1, motor1Config, motor1Homing, HALL_1);
        break;

    case Command::HOME_MOTOR2:
        commandSendResponse("HOMING MOTOR2 STARTED");
        homingStartXY(motor2, motor2Config, motor2Homing, HALL_2);
        break;

    case Command::HOME_MOTOR3:
        commandSendResponse("HOMING MOTOR3 STARTED");
        homingStartZ(motor3, motor3Config, motor3Homing, HALL_3);
        break;

    case Command::HOME_ALL:
        // 🔹 Inicializa secuencia HOME-ALL
        homeAllState = HomeAllState::MOTOR1;
        commandSendResponse("HOME ALL SEQUENCE STARTED");
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
        commandSendResponse("UNKNOWN COMMAND");
        break;
    }
}