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



def main_loop():
    global streaming
    global shoulder, elbow
    print(
        "Available commands:\n"
        "STATUS\n"          # Muestra el estatus de motores y sensores
        "RESET\n"           # Resetea el sistema
        "HOME1\n"           # Homing del motor1 + status
        "HOME2\n"           # Homing del motor2 + status
        "HOME3\n"           # Homing del motor3 + status
        "HOME\n"        # Homing de todos los motores 1-2-3 + status
        "ANGLES\n"          # Muestra angulos actuales del sensor 1 y 2
        "PICK\n"            # Mueve Z hacia abajo, agarra pieza y sube
        "PLACE\n"           # Mueve Z hacia abajo, suelta pieza y sube
        "MOVE\n"            # Mueve motores 1 y 2 a angulos ingresados
                            # usando el AS5600
        "SHOW-COMMANDS\n"
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

        # print(msg.decode().strip())

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

    # 🔹 2️⃣ Revisar si hay input de teclado sin bloquear
    rlist, _, _ = select.select([sys.stdin], [], [], 0)

    if rlist:
        cmd = sys.stdin.readline().strip()
        print(f"Input: {cmd}")

        # 🔥 MOVE
        if cmd == "MOVE":

            while True:
                try:
                    shoulder = float(input("Enter shoulder-angle: ").strip())
                    # print(f"Target shoulder: {shoulder}")
                    break
                except ValueError:
                    print("Invalid value. Enter a valid number.")

            while True:
                try:
                    elbow = float(input("Enter elbow-angle: ").strip())
                    # print(f"Target elbow: {elbow}")
                    break
                except ValueError:
                    print("Invalid value. Enter a valid number.")

            commands.send_command(f"MOVE {shoulder} {elbow}")
            return

        # 🔥 MOVE_FEEDBACK
        elif cmd == "MOVE_FEEDBACK":

            while True:
                try:
                    shoulder = float(input("Enter the target angle for the shoulder: ").strip())
                    print(f"Target angle for the shoulder: {shoulder}")
                    break
                except ValueError:
                    print("Invalid value. Enter a valid number.")

            while True:
                try:
                    elbow = float(input("Enter the target angle for the elbow: ").strip())
                    print(f"Target angle for the elbow: {elbow}")
                    break
                except ValueError:
                    print("Invalid value. Enter a valid number.")

            commands.send_command(f"MOVE_FEEDBACK {shoulder} {elbow}")
            return

        # 🔥 SHOW-COMMANDS
        elif cmd == "SHOW-COMMANDS":
            print(
                "Comandos disponibles:\n"
                "ANGLES\n"
                "HOME1\n"
                "HOME2\n"
                "HOME3\n"
                "HOME\n"
                "RESET\n"
                "STATUS\n"
                "PICK\n"
                "PLACE\n"
                "MOVE\n"
                "SHOW-COMMANDS\n"
            )

        # 🔥 TODOS LOS DEMÁS
        else:
            commands.send_command(cmd)

    time.sleep_ms(10)

if __name__ == "__main__":
    main_loop()
