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
    print(
        "Comandos disponibles:\n"
        "STATUS\n"
        "RESET_ERRORS\n"
        "HOME_MOTOR1\n"
        "HOME_MOTOR2\n"
        "GET_ANGLE_1\n"
        "GET_ANGLE_1_START\n"
        "GET_ANGLE_1_STOP\n"
    )

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
    # 🔹 2️⃣ Revisar si hay input de teclado sin bloquear
    rlist, _, _ = select.select([sys.stdin], [], [], 0)
    if rlist:
        cmd = sys.stdin.readline().strip()
        print(f"Input: {cmd}")
        commands.send_command(cmd)
        # aqui limpiar el input para que no quede en la consola
        if cmd == "GET_ANGLE_1_START":
            streaming = True
            print("\n⚡ Streaming iniciado\n")
        elif cmd == "GET_ANGLE_1_STOP":
            streaming = False
            print("\n⏹ Streaming detenido\n")
    time.sleep_ms(10)  # 10 ms → latencia mínima


if __name__ == "__main__":
    main_loop()
