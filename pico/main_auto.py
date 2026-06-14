"""
ZERO---CHESS
Raspberry Pi Controller
Botón + Stockfish + UART RP2040
"""

import time
import chess
import chess.engine
import serial
import RPi.GPIO as GPIO

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

engine = chess.engine.SimpleEngine.popen_uci(STOCKFISH_PATH)
board = chess.Board()

time.sleep(2)

# =========================
# GPIO SETUP
# =========================

GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

homing_requested = False

# =========================
# BOTÓN (DEBOUNCE SIMPLE)
# =========================


def check_button():
    global homing_requested

    if GPIO.input(BUTTON_PIN) == GPIO.LOW:
        if not homing_requested:
            print("🔄 HOMING solicitado (botón)")
            homing_requested = True
        time.sleep(0.2)

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

# =========================
# HOMING
# =========================


def do_homing():
    print("🤖 Ejecutando HOMING...")
    send_to_robot("HOMING")
    wait_done()

# =========================
# STOCKFISH
# =========================


def get_best_move():
    result = engine.play(board, chess.engine.Limit(time=THINK_TIME))
    return result.move.uci(), result.move

# =========================
# LOOP
# =========================


print("🤖 ZERO-CHESS READY")

do_homing()

while True:

    # 🔘 botón siempre activo
    check_button()

    # ejecutar homing si fue pedido
    if homing_requested:
        homing_requested = False
        do_homing()
        continue

    input("\nENTER = jugar")

    uci, move_obj = get_best_move()

    print("♟️ Stockfish:", uci)

    send_to_robot(uci)

    success = wait_done()

    if success:
        board.push(move_obj)
