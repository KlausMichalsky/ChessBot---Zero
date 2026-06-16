"""
KAYRON
main.py - Controlador para Raspberry Pi
Raspberry Pi Controller
Stockfish + UART RP2040 + HOMING automático
"""

import time
import chess
import chess.engine
import serial
from chess.engine import EngineTerminatedError

# =========================
# CONFIG
# =========================

SERIAL_PORT = "/dev/ttyACM0"
BAUDRATE = 115200
# STOCKFISH_PATH = "stockfish"
STOCKFISH_PATH = "/usr/games/stockfish"
THINK_TIME = 0.1

# =========================
# INIT HARDWARE
# =========================

print("🤖 Iniciando ZERO-CHESS...")

ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)

print("Esperando USB...")
time.sleep(5)

print("Iniciando Stockfish...")
t0 = time.time()

engine = chess.engine.SimpleEngine.popen_uci(
    STOCKFISH_PATH,
    timeout=30.0
)

print("Stockfish OK")
print(f"Tiempo de inicio: {time.time() - t0:.2f} segundos")

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
    return result.move.uci()


# =========================
# START
# =========================
print("🤖 ZERO-CHESS READY")

do_homing()

# =========================================
# CONFIG INICIAL
# =========================================

mode = input("¿Quién empieza? (1=Humano, 2=Robot): ").strip()

human_turn = (mode == "1")

print("\n♟️ Iniciando partida...\n")

# =========================================
# LOOP PRINCIPAL
# =========================================

while True:

    # =====================================================
    # TURNO DEL HUMANO
    # =====================================================

    if human_turn:

        move = input("\n♟️ Tu jugada (ej: e2e4) | q = salir: ").strip().lower()

        if move == "q":
            print("👋 Saliendo...")
            break

        try:
            human_move = chess.Move.from_uci(move)

            if human_move not in board.legal_moves:
                print("❌ Jugada ilegal")
                continue

        except ValueError:
            print("❌ Formato inválido")
            continue

        board.push(human_move)

        print("👤 Humano:", move)

        # ❌ IMPORTANTE:
        # NO mover pieza del humano físicamente aquí

        human_turn = False

    # =====================================================
    # TURNO DEL ROBOT
    # =====================================================

    else:

        stockfish_move = get_best_move()

        print("🤖 Stockfish:", stockfish_move)

        send_to_robot(stockfish_move)
        wait_done()

        send_to_robot("HOME")
        wait_done()

        board.push(chess.Move.from_uci(stockfish_move))

        human_turn = True

    # =====================================================
    # FIN DE PARTIDA
    # =====================================================

    if board.is_game_over():
        print("\n🏁 Fin de partida")
        print(board.result())
        break


try:
    engine.quit()
except EngineTerminatedError:
    pass

ser.close()
