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
bool commandAvailable() {
    return Serial1.available();
}

// MANDAR RESPUESTA DE STATUS DE TODOS LOS MOTORES Y SENSORES
// -----------------------------------------------------------------------
void commandSendStatusReport() {
    String report = "";
    report += motorStatus(MotorID::J1) + "\n";
    report += motorStatus(MotorID::J2) + "\n";
    report += motorStatus(MotorID::Z) + "\n";

    Serial1.print(report);
}

// LECTURA DE COMANDOS
// -----------------------------------------------------------------------
String readCommand() {
    static String buffer = "";
    while (Serial1.available()) {
        char c = Serial1.read();
        if (c == '\n') {
            String cmd = buffer;
            buffer = "";
            cmd.trim();
            return cmd;
        } else {
            buffer += c;
        }
    }
    return ""; // no hay comando completo todavía
}

// MAPEAR STRING a enum class Command
// -----------------------------------------------------------------------
Command parseCommand(const String &cmd) {
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

// PROCESAMIENTO DE COMANDOS
// -----------------------------------------------------------------------
void processCommand(const String &cmdStr) {
    String trimmedCmd = cmdStr;
    trimmedCmd.trim();

    Command cmd = parseCommand(trimmedCmd);

    switch (cmd) {
        case Command::STATUS:
            commandSendStatusReport();
            break;

        case Command::RESET:
            motor1Homing.fault = false;
            motor2Homing.fault = false;
            motor3Homing.fault = false;
            homingInitXY(motor1Homing);
            homingInitXY(motor2Homing);
            homingInitZ(motor3Homing);
            homeAllState = HomeAllState::IDLE; // si estabas en HOME-ALL, cancelalo
            Serial1.println("SYSTEM RESET");
            break;

        case Command::HOME1:
            Serial1.println("HOMING MOTOR1 STARTED");
            homingStartXY(motor1, motor1Config, motor1Homing, HALL_1);
            break;

        case Command::HOME2:
            Serial1.println("HOMING MOTOR2 STARTED");
            homingStartXY(motor2, motor2Config, motor2Homing, HALL_2);
            break;

        case Command::HOME3:
            Serial1.println("HOMING MOTOR3 STARTED");
            homingStartZ(motor3, motor3Config, motor3Homing, HALL_3);
            break;

        case Command::HOME_ALL:
            Serial1.println("HOME ALL SEQUENCE STARTED");
            homeAllState = HomeAllState::MOTOR1;
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
            Serial1.println("UNKNOWN COMMAND");
            break;
    }
}