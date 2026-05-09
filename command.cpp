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

#include "Z_Axis.h"
#include "command.h"
#include "config.h"
#include "core.h"
#include "homing.h"
#include "motors.h"
#include "sensors.h"
#include "utils.h"
#include "xy_plane.h"

// -----------------------------------------------------------------------
// VARIABLES EXTERNAS
// -----------------------------------------------------------------------
extern HomingXY motor1Homing;
extern HomingXY motor2Homing;
extern HomingZ motor3Homing;
extern bool homeAllActive;

// -----------------------------------------------------------------------
// COMPROBACIÓN DE COMANDOS DISPONIBLES
// -----------------------------------------------------------------------
bool commandAvailable() {
    return Serial1.available();
}

// -----------------------------------------------------------------------
// MANDAR RESPUESTA DE STATUS DE TODOS LOS MOTORES Y SENSORES
// -----------------------------------------------------------------------
void commandSendStatusReport() {
    String report = "";
    report += motorStatus(MotorID::J1) + "\n";
    report += motorStatus(MotorID::J2) + "\n";
    report += motorStatus(MotorID::Z) + "\n";

    Serial1.print(report);
}

// -----------------------------------------------------------------------
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

// -----------------------------------------------------------------------
// MAPEAR STRING a enum class Command
// -----------------------------------------------------------------------
Command parseCommand(const String &cmd) {
    if (cmd == "STATUS")
        return Command::STATUS;
    else if (cmd == "RESET")
        return Command::RESET;
    else if (cmd == "HOME1")
        return Command::HOME1;
    else if (cmd == "HOME2")
        return Command::HOME2;
    else if (cmd == "HOME3")
        return Command::HOME3;
    else if (cmd == "HOME")
        return Command::HOME;
    else if (cmd == "ANGLES")
        return Command::ANGLES;
    else if (cmd == "PICK")
        return Command::PICK;
    else if (cmd == "PLACE")
        return Command::PLACE;
    else if (cmd.startsWith("MOVE"))
        return Command::MOVE;
    else if (cmd == "SHOW-COMMANDS")
        return Command::SHOW_COMMANDS;
    else
        return Command::UNKNOWN;
}

// -----------------------------------------------------------------------
// PROCESAMIENTO DE COMANDOS
// -----------------------------------------------------------------------
void processCommand(const String &cmdStr) {
    String trimmedCmd = cmdStr;
    trimmedCmd.trim(); // eliminar espacios al iniciofinal,
                       // justo después de crear trimmedCmd

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
            homeSingleState = HomeSingleState::RUNNING;
            homingStartXY(motor1, motor1Config, motor1Homing, HALL_1);
            break;

        case Command::HOME2:
            Serial1.println("HOMING MOTOR2 STARTED");
            homeSingleState = HomeSingleState::RUNNING;
            homingStartXY(motor2, motor2Config, motor2Homing, HALL_2);
            break;

        case Command::HOME3:
            Serial1.println("HOMING MOTOR3 STARTED");
            homeSingleState = HomeSingleState::RUNNING;
            homingStartZ(motor3, motor3Config, motor3Homing, HALL_3);
            break;

        case Command::HOME:
            Serial1.println("HOME SEQUENCE STARTED");
            homeAllState = HomeAllState::MOTOR1;
            break;

        case Command::ANGLES:
            Serial1.print("ANGLE1: ");
            Serial1.println(rawToDegrees(sensorReadRawAngle(Wire)));
            Serial1.print("ANGLE2: ");
            Serial1.println(rawToDegrees(sensorReadRawAngle(Wire1)));
            Serial1.println();
            break;

        case Command::PICK:
            Serial1.println("PICKING PIECE!");
            zPick(); // bloqueante: sube/baja Z y activa imán
                     // después de esto, XY puede moverse sin problemas
            break;

        case Command::PLACE:
            Serial1.println("PLACING PIECE!");
            zPlace();
            break;

        case Command::MOVE: {
            float targetShoulderAngle;
            float targetElbowAngle;

            int parsed = sscanf(trimmedCmd.c_str(), "MOVE %f %f",
                                &targetShoulderAngle,
                                &targetElbowAngle);

            if (parsed != 2) {
                Serial1.println("ERROR: MOVE format invalid");
                break;
            }

            moveToAngles(targetShoulderAngle, targetElbowAngle);

            break;
        }

        case Command::SHOW_COMMANDS:
            break;

        default:
            Serial1.println("UNKNOWN COMMAND");
            break;
    }
}