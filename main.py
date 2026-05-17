# ========================================================================
#                 🔹 C H E S S B O T  -  Z E R O 🔹
# ========================================================================
#   Archivo    : main.py
#   Autor      : Klaus Michalsky
#   Fecha      : Feb. 2026
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       - Programa principal del Pi
#       - Interfaz de usuario para enviar comandos
#       - Mostrar resultados y respuestas del RP2040.
# ========================================================================


import time
import sys
import select

import commands
import communication


streaming = False
board_done = False


def main_loop():
    global streaming
    global shoulder, elbow

    print(
        "Comandos disponibles:\n"
        "ANGLES\n"
        "HOME\n"
        "HOME1\n"
        "HOME2\n"
        "HOME3\n"
        "MOVE\n"
        "PICK\n"
        "PLACE\n"
        "BOARD\n"
        "RESET\n"
        "COMMANDS\n"
        "STATUS\n"
    )

    print("Command: ")

    while True:
        # 🔹 1️⃣ Leer UART siempre
        if communication.any():
            msg = communication.readline()
            if msg:
                clean_msg = msg.decode('utf-8', 'ignore').rstrip('\r\n')
                print(clean_msg)

        keyboard_input()


def readUart():
    if communication.any():
        msg = communication.readline()
        if msg:
            clean_msg = msg.decode('utf-8', 'ignore').rstrip('\r\n')
            print(clean_msg)


def keyboard_input():
    global streaming

    rlist, _, _ = select.select([sys.stdin], [], [], 0)

    if not rlist:
        return

    cmd = sys.stdin.readline().strip().upper()
    print(f"Input: {cmd}")

    # =========================================================
    # 🔥 MOVE
    # =========================================================
    if cmd == "MOVE":

        while True:
            capture = input("Capture? (Y/N): ").strip().upper()
            # esta el input del usuar que se guarda en capture dentro de la lista de opciones validas?
            if capture in ["Y", "N"]:
                break
            print("Invalid value. Enter Y or N.")

        # =========================================================
        # START SQUARE
        # =========================================================
        while True:
            start_square = input("Start square: ").strip().upper()

            if len(start_square) != 2 or start_square[0] < 'A' or start_square[0] > 'H' or start_square[1] < '1' or start_square[1] > '8':
                print(f"Invalid start square: {start_square}")
                continue
            break

        # =========================================================
        # END SQUARE
        # =========================================================
        while True:
            end_square = input("End square: ").strip().upper()

            if len(end_square) != 2 or end_square[0] < 'A' or end_square[0] > 'H' or end_square[1] < '1' or end_square[1] > '8':
                print(f"Invalid end square: {end_square}")
                continue
            break

        if capture == "Y":
            commands.send_command(f"CAPTURE {start_square} {end_square}")
        else:
            commands.send_command(f"MOVE {start_square} {end_square}")
        return

    # =========================================================
    # 🔥 SQUARE
    # =========================================================
    elif cmd == "SQUARE":

        while True:
            square = input("Enter square (example E4): ").strip().upper()

            if len(square) != 2:
                print("Invalid square length.")
                continue

            file = square[0]
            rank = square[1]

            if file < 'A' or file > 'H':
                print("Invalid file. Use A-H.")
                continue

            if rank < '1' or rank > '8':
                print("Invalid rank. Use 1-8.")
                continue

            break

        commands.send_command(f"SQUARE {square}")
        return

    # =========================================================
    # 🔥 SHOW COMMANDS
    # =========================================================
    elif cmd == "SHOW-COMMANDS":
        print(
            "Comandos disponibles:\n"
            "ANGLES\n"
            "HOME\n"
            "HOME1\n"
            "HOME2\n"
            "HOME3\n"
            "MOVE\n"
            "PICK\n"
            "PLACE\n"
            "BOARD\n"
            "RESET\n"
            "COMMANDS\n"
            "SQUARE\n"
            "STATUS\n"
        )
        return

    # =========================================================
    # 🔥 DEFAULT
    # =========================================================
    else:
        commands.send_command(cmd)
        return


if __name__ == "__main__":
    main_loop()
