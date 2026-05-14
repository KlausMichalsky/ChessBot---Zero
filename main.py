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
import ikinematics
import square_xy


streaming = False
board_done = False


def main_loop():
    global streaming
    global shoulder, elbow

    print(
        "Comandos disponibles:\n"
        "ANGLES\n"          # Muestra angulos actuales del sensor 1 y 2
        "HOME\n"        # Homing de todos los motores 1-2-3 + status
        "HOME1\n"           # Homing del motor1 + status
        "HOME2\n"           # Homing del motor2 + status
        "HOME3\n"           # Homing del motor3 + status
        "MOVE\n"            # Mueve motores 1 y 2 a angulos ingresados
        "PICK\n"            # Mueve Z hacia abajo, agarra pieza y sube
        "PLACE\n"           # Mueve Z hacia abajo, suelta pieza y sube
        "BOARD\n"           # Imprime coordenadas del tablero
        "RESET\n"           # Resetea el sistema
        "COMMANDS\n"        # Muestra lista de comandos
        "STATUS\n"          # Muestra el estatus de motores y sensores
    )

    print("Command: ")

    while True:
        # 🔹 1️⃣ Leer UART siempre
        if communication.any():
            msg = communication.readline()
            if msg:
                # decodificar y eliminar \r, \n, espacios
                # print(msg)  # mostrar mensaje sin procesar
                clean_msg = msg.decode('utf-8', 'ignore').rstrip('\r\n')
                print(clean_msg)

        keyboard_input()  # revisar input de teclado sin bloquear


def readUart():
    if communication.any():
        msg = communication.readline()
        if msg:
            # decodificar y eliminar \r, \n, espacios
            # print(msg)  # mostrar mensaje sin procesar
            clean_msg = msg.decode('utf-8', 'ignore').rstrip('\r\n')
            # clean_msg = print(msg.decode().strip())
            print(clean_msg)


def keyboard_input():
    global streaming

    # 🔹 Revisar si hay input de teclado sin bloquear
    rlist, _, _ = select.select([sys.stdin], [], [], 0)

    if rlist:
        cmd = sys.stdin.readline().strip()
        print(f"Input: {cmd}")

        match cmd:

            # 🔥 MOVE
            case "MOVE":

                while True:
                    try:
                        shoulder = float(
                            input("Enter shoulder-angle: ").strip())
                        break
                    except ValueError:
                        print("Invalid value. Enter a valid number.")

                while True:
                    try:
                        elbow = float(input("Enter elbow-angle: ").strip())
                        break
                    except ValueError:
                        print("Invalid value. Enter a valid number.")

                commands.send_command(f"MOVE {shoulder} {elbow}")
                return

            # 🔥 SQUARE
            case "SQUARE":

                while True:
                    square = input(
                        "Enter square (example E4): ").strip().upper()

                    # VALIDAR LONGITUD
                    if len(square) != 2:
                        print("Invalid square length.")
                        continue

                    file = square[0]
                    rank = square[1]

                    # VALIDAR LETRA
                    if file < 'A' or file > 'H':
                        print("Invalid file. Use A-H.")
                        continue

                    # VALIDAR NUMERO
                    if rank < '1' or rank > '8':
                        print("Invalid rank. Use 1-8.")
                        continue

                    break

                commands.send_command(f"SQUARE {square}")
                return

            # 🔥 SHOW-COMMANDS
            case "SHOW-COMMANDS":
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

            # 🔥 DEFAULT
            case _:
                commands.send_command(cmd)

    time.sleep_ms(10)


if __name__ == "__main__":
    main_loop()
