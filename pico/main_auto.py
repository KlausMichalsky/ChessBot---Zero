"""
ZERO-CHESS
Raspberry Pi Controller
Stockfish + UART RP2040 + HOMING automático
"""

import time
import chess
import chess.engine
import serial

# =========================
# CONFIG
# =========================

SERIAL_PORT = "/dev/ttyACM0"
BAUDRATE = 115200
STOCKFISH_PATH = "stockfish"
# STOCKFISH_PATH = "/usr/games/stockfish"
THINK_TIME = 0.1

# =========================
# INIT HARDWARE
# =========================

print("🤖 Iniciando ZERO-CHESS...")

ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)

engine = chess.engine.SimpleEngine.popen_uci(
    STOCKFISH_PATH,
    timeout=10.0
)
board = chess.Board()

time.sleep(2)  # estabilizar USB

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
                break

# =========================
# HOMING AUTOMÁTICO
# =========================


def do_homing():
    print("🤖 Enviando HOMING...")
    ser.write(b"HOMING\n")

    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if line:
            print("RP2040:", line)
            if "DONE" in line:
                print("✅ HOMING COMPLETO")
                break

# =========================
# STOCKFISH
# =========================


def get_best_move():
    result = engine.play(board, chess.engine.Limit(time=THINK_TIME))
    move = result.move
    board.push(move)
    return move.uci()

# =========================
# START
# =========================


print("🤖 ZERO-CHESS READY")

do_homing()

# =========================
# LOOP PRINCIPAL
# =========================

while True:

    input("\nENTER = jugar")

    move = get_best_move()

    print("♟️ Stockfish:", move)

    send_to_robot(move)

    wait_done()
