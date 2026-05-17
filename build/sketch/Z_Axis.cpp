#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\z_Axis.cpp"
#include <Arduino.h>

#include "config.h"
#include "homing.h"
#include "motors.h"
#include "z_axis.h"

// =========================================================
// ESTADO GLOBAL Z
// =========================================================
ZState zState = ZState::IDLE;

// =========================================================
// MOVIMIENTOS BASE (NO BLOQUEANTES)
// =========================================================
void zMoveDown() {
    motor3.setMaxSpeed(6000);
    motor3.setAcceleration(15000);
    motor3.moveTo(Z_STEPS_DOWN);
}

void zMoveUp() {
    motor3.setMaxSpeed(6000);
    motor3.setAcceleration(15000);
    motor3.moveTo(0);
}

// =========================================================
// IMÁN
// =========================================================
void magnetON() {
    digitalWrite(MAGNET, HIGH);
}

void magnetOFF() {
    digitalWrite(MAGNET, LOW);
}

// =========================================================
// INICIO DE SECUENCIAS
// =========================================================
void startZPick() {
    motorEnableZ();
    zState = ZState::PICK_DOWN;
    zMoveDown();
}

void startZPlace() {
    motorEnableZ();
    zState = ZState::PLACE_DOWN;
    zMoveDown();
}

// =========================================================
// UPDATE Z STATE MACHINE
// =========================================================
void updateZ() {
    switch (zState) {
        // =====================================================
        // PICK DOWN
        // =====================================================
        case ZState::PICK_DOWN:

            motor3.run();

            if (motor3.distanceToGo() == 0) {
                Serial1.println("Z: GRIP");
                magnetON();
                zState = ZState::PICK_GRIP;
                zMoveUp(); // siguiente acción inmediata
            }
            break;

        // =====================================================
        // PICK GRIP
        // =====================================================
        case ZState::PICK_GRIP:

            motor3.run();

            if (motor3.distanceToGo() == 0) {
                zState = ZState::PICK_UP;
            }
            break;

        // =====================================================
        // PICK UP
        // =====================================================
        case ZState::PICK_UP:

            if (motor3.distanceToGo() == 0) {
                Serial1.println("Z PICK DONE");
                zState = ZState::IDLE;
            }
            break;

        // =====================================================
        // PLACE DOWN
        // =====================================================
        case ZState::PLACE_DOWN:

            motor3.run();

            if (motor3.distanceToGo() == 0) {
                Serial1.println("Z: RELEASE");
                magnetOFF();
                zState = ZState::PLACE_RELEASE;
                zMoveUp();
            }
            break;

        // =====================================================
        // PLACE RELEASE
        // =====================================================
        case ZState::PLACE_RELEASE:

            motor3.run();

            if (motor3.distanceToGo() == 0) {
                zState = ZState::PLACE_UP;
            }
            break;

        // =====================================================
        // PLACE UP
        // =====================================================
        case ZState::PLACE_UP:

            if (motor3.distanceToGo() == 0) {
                Serial1.println("Z PLACE DONE");
                zState = ZState::IDLE;
            }
            break;

        // =====================================================
        // IDLE
        // =====================================================
        case ZState::IDLE:
        default:
            break;
    }
}