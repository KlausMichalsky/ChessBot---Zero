"""
ZERO---CHESS
Raspberry Pi Controller
Botón + Stockfish + UART RP2040
"""

import time
import chess
import chess.engine
import serial
from gpiozero import Button

# =========================
# CONFIG
# =========================

SERIAL_PORT = "/dev/ttyACM0"
BAUDRATE = 115200
STOCKFISH_PATH = "/usr/games/stockfish"
THINK_TIME = 0.1

BUTTON_PIN = 17

# =========================
# INIT
# =========================

ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
button = Button(BUTTON_PIN, bounce_time=0.2)

engine = chess.engine.SimpleEngine.popen_uci(STOCKFISH_PATH)
board = chess.Board()

time.sleep(2)

# =========================
# FLAGS
# =========================

homing_requested = False

# =========================
# BOTÓN
# =========================


def request_homing():
    global homing_requested
    print("🔄 HOMING solicitado")
    homing_requested = True


button.when_pressed = request_homing

# =========================
# UART HELPERS
# =========================


def send_to_robot(cmd):
    ser.write((cmd + "\n").encode())


def wait_done():
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if line:
            print("RP2040:", line)
            if "DONE" in line:
                return True
            if "ERROR" in line:
                return False

# =========================
# HOMING
# =========================


def do_homing():
    print("🤖 Ejecutando HOMING...")
    send_to_robot("HOMING")
    return wait_done()

# =========================
# RESET GAME
# =========================


def reset_game():
    global board
    print("♻️ Reiniciando partida...")
    board = chess.Board()
    return do_homing()

# =========================
# STOCKFISH
# =========================


def get_best_move():
    result = engine.play(board, chess.engine.Limit(time=THINK_TIME))
    return result.move.uci(), result.move

# =========================
# LOOP PRINCIPAL
# =========================


print("🤖 ZERO-CHESS READY")

# HOMING inicial
do_homing()

while True:

    # HOMING por botón
    if homing_requested:
        homing_requested = False
        do_homing()
        continue

    cmd = input("\nENTER = jugar | r = reset | q = salir: ").strip().lower()

    if cmd == "q":
        print("👋 Saliendo...")
        break

    if cmd == "r":
        reset_game()
        continue

    # jugar movimiento
    uci, move_obj = get_best_move()

    print("♟️ Stockfish:", uci)

    send_to_robot(uci)

    success = wait_done()

    if success:
        board.push(move_obj)
    else:
        print("⚠️ Movimiento falló, no se actualiza tablero")
