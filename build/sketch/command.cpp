#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\command.cpp"
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
String commandReport(MotorID id)
{
    String status = "";

    switch (id)
    {
    case MotorID::J1: // motor1 XY
        switch (motor1Homing.state)
        {
        case HomingStateXY::OK:
            status += "MOTOR1 OK (" + String(motor1Homing.centerPosition) + "); ";
            break;
        case HomingStateXY::ERROR:
            status += "MOTOR1 ERROR; ";
            break;
        case HomingStateXY::INACTIVE:
            break; // nada que mostrar
        default:
            status += "MOTOR1 RUNNING; ";
            break;
        }
        break;

    case MotorID::J2: // motor2 XY
        switch (motor2Homing.state)
        {
        case HomingStateXY::OK:
            status += "MOTOR2 OK (" + String(motor2Homing.centerPosition) + "); ";
            break;
        case HomingStateXY::ERROR:
            status += "MOTOR2 ERROR; ";
            break;
        case HomingStateXY::INACTIVE:
            break;
        default:
            status += "MOTOR2 RUNNING; ";
            break;
        }
        break;

    case MotorID::Z: // motor Z
        switch (motor3Homing.state)
        {
        case HomingStateZ::OK:
            status += "MOTOR3 OK (" + String(motor3Homing.reference) + "); ";
            break;
        case HomingStateZ::ERROR:
            status += "MOTOR3 ERROR; ";
            break;
        case HomingStateZ::INACTIVE:
            break;
        default:
            status += "MOTOR3 RUNNING; ";
            break;
        }
        break;
    }

    if (status == "")
        status = "IDLE";

    return status;
}

String commandStatusReport()
{
    String status = "";

    // Motor1
    switch (motor1Homing.state)
    {
    case HomingStateXY::OK:
        status += "MOTOR1 OK; ";
        break;
    case HomingStateXY::ERROR:
        status += "MOTOR1 ERROR; ";
        break;
    case HomingStateXY::INACTIVE:
        break; // nada que mostrar
    default:
        status += "MOTOR1 RUNNING; ";
        break;
    }

    // Motor2
    switch (motor2Homing.state)
    {
    case HomingStateXY::OK:
        status += "MOTOR2 OK; ";
        break;
    case HomingStateXY::ERROR:
        status += "MOTOR2 ERROR; ";
        break;
    case HomingStateXY::INACTIVE:
        break;
    default:
        status += "MOTOR2 RUNNING; ";
        break;
    }

    // Motor3
    switch (motor3Homing.state)
    {
    case HomingStateZ::OK:
        status += "MOTOR3 OK; ";
        break;
    case HomingStateZ::ERROR:
        status += "MOTOR3 ERROR; ";
        break;
    case HomingStateZ::INACTIVE:
        break;
    default:
        status += "MOTOR3 RUNNING; ";
        break;
    }

    if (status == "")
        status = "IDLE"; // si ningún motor tiene actividad

    return status;
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
    if (cmd == "HOME1")
        return Command::HOME1;
    if (cmd == "HOME2")
        return Command::HOME2;
    if (cmd == "HOME3")
        return Command::HOME3;
    if (cmd == "HOME-ALL")
        return Command::HOME_ALL;
    if (cmd == "ANGLE1")
        return Command::ANGLE1;
    if (cmd == "ANGLE1-STREAM")
        return Command::ANGLE1_STREAM;
    if (cmd == "ANGLE2")
        return Command::ANGLE2;
    if (cmd == "ANGLE2-STREAM")
        return Command::ANGLE2_STREAM;
    if (cmd == "STOP-STREAM")
        return Command::STOP_STREAM;
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
        motor3Homing.fault = false;
        homingInitXY(motor1Homing);
        homingInitXY(motor2Homing);
        homingInitZ(motor3Homing);
        homeAllState = HomeAllState::IDLE; // si estabas en HOME-ALL, cancelalo
        commandSendResponse("RESET");
        break;

    case Command::HOME1:
        commandSendResponse("HOMING MOTOR1 STARTED");
        homingStartXY(motor1, motor1Config, motor1Homing, HALL_1);
        break;

    case Command::HOME2:
        commandSendResponse("HOMING MOTOR2 STARTED");
        homingStartXY(motor2, motor2Config, motor2Homing, HALL_2);
        break;

    case Command::HOME3:
        commandSendResponse("HOMING MOTOR3 STARTED");
        homingStartZ(motor3, motor3Config, motor3Homing, HALL_3);
        break;

    case Command::HOME_ALL:
        // 🔹 Inicializa secuencia HOME-ALL
        homeAllState = HomeAllState::MOTOR1;
        commandSendResponse("HOME ALL SEQUENCE STARTED");
        break;

    case Command::ANGLE1:
        Serial1.print("ANGLE1: ");
        sensorSendAngle(Wire); // Leer y enviar ángulo del primer sensor
        break;

    case Command::ANGLE1_STREAM:
        dynamicAngle1 = true; // Activar lectura continua
        break;

    case Command::ANGLE2:
        Serial1.print("ANGLE2: ");
        sensorSendAngle(Wire1); // Leer y enviar ángulo del segundo sensor
        break;

    case Command::ANGLE2_STREAM:
        dynamicAngle2 = true; // Activar lectura continua
        break;

    case Command::STOP_STREAM:
        dynamicAngle1 = false;
        dynamicAngle2 = false;
        break;

    case Command::UNKNOWN:
    default:
        commandSendResponse("UNKNOWN COMMAND");
        break;
    }
}