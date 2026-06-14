// =======================================================================
//                          🔹 K A Y R O N 🔹
// =======================================================================
//  Archivo    : command.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Definición de funciones para la gestión de comandos
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

// VARIABLES EXTERNAS
// -----------------------------------------------------------------------
extern HomingXY motor1Homing;
extern HomingXY motor2Homing;
extern HomingZ motor3Homing;
extern bool homeAllActive;

// COMPROBACIÓN DE COMANDOS DISPONIBLES
// -----------------------------------------------------------------------
bool commandAvailable() {
    return Serial1.available();
}

// ENVIO DE RESPUESTA DE STATUS DE TODOS LOS MOTORES Y SENSORES
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

// MAPEO DE STRING A ENUM CLASS COMMAND
// -----------------------------------------------------------------------
Command parseCommand(const String &cmd) {
    if (cmd == "STATUS")
        return Command::STATUS;
    else if (cmd == "RESET")
        return Command::RESET;
    else if (cmd == "ANGLES")
        return Command::ANGLES;
    else if (cmd == "BOARD")
        return Command::BOARD;
    else if (cmd == "COMMANDS")
        return Command::COMMANDS;
    else if (cmd == "HOME")
        return Command::HOME;
    else if (cmd == "HOMING")
        return Command::HOMING;
    else if (cmd.startsWith("MOVE"))
        return Command::MOVE;
    else if (cmd.startsWith("SQUARE"))
        return Command::SQUARE;
    else
        return Command::UNKNOWN;
    // else if (cmd == "HOME1")
    //     return Command::HOME1;
    // else if (cmd == "HOME2")
    //     return Command::HOME2;
    // else if (cmd == "HOME3")
    //     return Command::HOME3;
    // else if (cmd == "PICK")
    //     return Command::PICK;
    // else if (cmd == "PLACE")
    //     return Command::PLACE;
    // else if (cmd.startsWith("CAPTURE"))
    //     return Command::CAPTURE;
}

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
            homeAllState = HomeAllState::IDLE; // si estabas en HOMING-ALL, cancelalo
            Serial1.println("SYSTEM RESET");
            break;

        case Command::ANGLES:
            Serial1.print("ANGLE1: ");
            Serial1.println(rawToDegrees(sensorReadRawAngle(Wire)), 1);
            Serial1.print("ANGLE2: ");
            Serial1.println(rawToDegrees(sensorReadRawAngle(Wire1)), 1);
            Serial1.println();
            break;

        case Command::BOARD:
            printBoardXY();
            break;

        case Command::COMMANDS:
            break;

        case Command::HOME:
            moveToHomeXY();
            break;

        case Command::HOMING:
            Serial1.println("HOMING SEQUENCE STARTED");
            homeAllState = HomeAllState::MOTOR1;
            break;

        case Command::MOVE: {
            char startSquare[4] = {0};
            char endSquare[4] = {0};

            int parsed = sscanf(trimmedCmd.c_str(),
                                "MOVE %3s %3s",
                                startSquare,
                                endSquare);

            if (parsed != 2) {
                Serial1.println("ERROR");
                break;
            }

            if (strcmp(startSquare, endSquare) == 0) {
                Serial1.println("ERROR SAME SQUARE");
                break;
            }

            float s1, s2;
            float e1, e2;

            if (!chessSquareToAngles(String(startSquare), s1, s2)) {
                Serial1.println("ERROR START");
                break;
            }

            if (!chessSquareToAngles(String(endSquare), e1, e2)) {
                Serial1.println("ERROR END");
                break;
            }

            // SOLO UNA LLAMADA
            startMoveSequence(s1, s2, e1, e2);

            break;
        }

        default:
            Serial1.println("UNKNOWN COMMAND");
            break;

            // case Command::HOME1:
            //     Serial1.println("HOMING MOTOR1 STARTED");
            //     homeSingleState = HomeSingleState::RUNNING;
            //     homingStartXY(motor1, motor1Config, motor1Homing, HALL_1);
            //     break;

            // case Command::HOME2:
            //     Serial1.println("HOMING MOTOR2 STARTED");
            //     homeSingleState = HomeSingleState::RUNNING;
            //     homingStartXY(motor2, motor2Config, motor2Homing, HALL_2);
            //     break;

            // case Command::HOME3:
            //     Serial1.println("HOMING MOTOR3 STARTED");
            //     homeSingleState = HomeSingleState::RUNNING;
            //     homingStartZ(motor3, motor3Config, motor3Homing, HALL_3);
            //     break;

            // case Command::PICK:
            //     Serial1.println("PICKING PIECE!");
            //     // zPick(); // bloqueante: sube/baja Z y activa imán
            //     // después de esto, XY puede moverse sin problemas
            //     break;

            // case Command::PLACE:
            //     Serial1.println("PLACING PIECE!");
            //     // zPlace();
            //     break;

            // case Command::CAPTURE: {
            //     char startSquare[4] = {0};
            //     char endSquare[4] = {0};

            //     int parsed = sscanf(trimmedCmd.c_str(),
            //                         "CAPTURE %3s %3s",
            //                         startSquare,
            //                         endSquare);

            //     if (parsed != 2) {
            //         Serial1.println("ERROR: CAPTURE format invalid");
            //         break;
            //     }

            //     if (strcmp(startSquare, endSquare) == 0) {
            //         Serial1.println("ERROR: Same square");
            //         break;
            //     }

            //     Serial1.print("CAPTURE FROM: ");
            //     Serial1.print(startSquare);
            //     Serial1.print(" TO: ");
            //     Serial1.println(endSquare);
            //     Serial1.println();

            //     break;
            // }

            // case Command::SQUARE: {
            //     char square[3];

            //     int parsed = sscanf(trimmedCmd.c_str(), "SQUARE %2s", square);
            //     // "SQUARE %2s" leer máximo 2 caracteres.

            //     if (parsed != 1) {
            //         Serial1.println("ERROR: SQUARE format invalid");
            //         break;
            //     }

            //     float shoulderDeg;
            //     float elbowDeg;

            //     // CASILLA -> ANGULOS
            //     if (!chessSquareToAngles(
            //             String(square),
            //             shoulderDeg,
            //             elbowDeg)) {
            //         Serial1.println("ERROR: Invalid square or unreachable");
            //         break;
            //     }

            //     // MOVER BRAZO
            //     moveToAngles(shoulderDeg, elbowDeg);
            //     break;
            // }
    }
}