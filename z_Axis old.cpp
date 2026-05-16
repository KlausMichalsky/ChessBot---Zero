// // =======================================================================
// //                 🔹 C H E S S B O T  —   Z E R O 🔹
// // =======================================================================
// //  Archivo    : z_axis.cpp
// //  Autor      : Klaus Michalsky
// //  Fecha      : Feb-2026
// // -----------------------------------------------------------------------
// //  ▫️ DESCRIPCIÓN
// //      - Definicion de las funciones para controlar el eje Z
// // =======================================================================

// #include <Arduino.h>

// #include "config.h"
// #include "homing.h"
// #include "motors.h"
// #include "z_axis.h"

// // VARIABLES LOCALES
// // -----------------------------------------------------------------------
// static bool zMoving = false;
// static long zTarget = 0;

// ZState zState = ZState::IDLE;

// // MOVIMIENTO DEL EJE Z
// // -----------------------------------------------------------------------
// void zStep() {
//     if (zMoving) {
//         motor3.run(); // AccelStepper mueve y maneja aceleración
//         if (motor3.distanceToGo() == 0) {
//             zMoving = false;
//         }
//     }
// }

// // ASIGNACION DE DESTINO HACIA ABAJO (NO MUEVE TODAVIA)
// // -----------------------------------------------------------------------
// // void zMoveDown() {
// //     motor3.setMaxSpeed(6000);
// //     motor3.setAcceleration(15000);
// //     motor3.setCurrentPosition(0);
// //     zTarget = Z_STEPS_DOWN; // Z_STEPS_DOWN pasos definido en config.h
// //     motor3.moveTo(zTarget);
// //     zMoving = true; // activa zMoving para que zStep() lo ejecute.
// //     while (zMoving)
// //         zStep();
// // }
// void zMoveDown() {
//     motor3.setMaxSpeed(6000);
//     motor3.setAcceleration(15000);

//     motor3.moveTo(Z_STEPS_DOWN);
// }

// // ASIGNACION DE DESTINO HACIA ARRIBA (NO MUEVE TODAVIA)
// // -----------------------------------------------------------------------
// // void zMoveUp() {
// //     motor3.setMaxSpeed(6000);
// //     motor3.setAcceleration(15000);
// //     zTarget = 0;
// //     motor3.moveTo(zTarget);
// //     zMoving = true;
// //     while (zMoving)
// //         zStep();
// // }
// void zMoveUp() {
//     motor3.setMaxSpeed(6000);
//     motor3.setAcceleration(15000);

//     motor3.moveTo(0);
// }

// // CONTROL DEL IMAN
// // -----------------------------------------------------------------------
// void magnetON() {
//     digitalWrite(MAGNET, HIGH);
// }

// void magnetOFF() {
//     digitalWrite(MAGNET, LOW);
// }

// // starsZPick ZPlace SON BLOQUEANTES Y SE EJECUTAN DESDE COMMAND.CPP
// // -----------------------------------------------------------------------
// // void zPick() {
// //     motorEnableZ();
// //     magnetOFF();
// //     zMoveDown();
// //     delay(Z_DELAY); // para mejorar controal al agarrar y soltar
// //     magnetON();
// //     delay(Z_DELAY);
// //     zMoveUp();
// //     motorDisableZ();
// // }

// // void zPlace() {
// //     motorEnableZ();
// //     zMoveDown();
// //     delay(Z_DELAY);
// //     magnetOFF();
// //     delay(Z_DELAY);
// //     zMoveUp();
// //     motorDisableZ();
// // }
// void startZPick() {
//     motorEnableZ();
//     zState = ZState::PICK_DOWN;
//     zMoveDown();
// }

// void startZPlace() {
//     motorEnableZ();
//     zState = ZState::PLACE_DOWN;
//     zMoveDown();
// }

// void updateZ() {
//     switch (zState) {
//             // =========================================================
//             // 🔹 PICK FLOW
//             // =========================================================

//         case ZState::PICK_DOWN:

//             Serial1.println("MOVE Z DOWN");
//             zMoveDown();

//             if (motor3.distanceToGo() == 0) {
//                 Serial1.println("PICK");
//                 magnetON();
//                 zState = ZState::PICK_GRIP;
//             }
//             break;

//         case ZState::PICK_GRIP:

//             Serial1.println("MOVE Z UP");
//             zMoveUp();
//             zState = ZState::PICK_UP;
//             break;

//         case ZState::PICK_UP:

//             if (motor3.distanceToGo() == 0) {
//                 zState = ZState::IDLE;
//                 Serial1.println("Z PICK DONE");
//             }
//             break;

//             // =========================================================
//             // 🔹 PLACE FLOW
//             // =========================================================

//         case ZState::PLACE_DOWN:

//             Serial1.println("MOVE Z DOWN");
//             zMoveDown();

//             if (motor3.distanceToGo() == 0) {
//                 Serial1.println("PLACE");
//                 magnetOFF();
//                 zState = ZState::PLACE_RELEASE;
//                 Serial1.println("MOVE Z UP");
//                 zMoveUp();
//             }
//             break;

//         case ZState::PLACE_RELEASE:

//             if (motor3.distanceToGo() == 0) {
//                 zState = ZState::PLACE_UP;
//             }
//             break;

//         case ZState::PLACE_UP:

//             if (motor3.distanceToGo() == 0) {
//                 zState = ZState::IDLE;
//                 Serial1.println("Z PLACE DONE");
//             }
//             break;

//             // =========================================================
//             // 🔹 IDLE
//             // =========================================================

//         case ZState::IDLE:
//         default:
//             break;
//     }
// }