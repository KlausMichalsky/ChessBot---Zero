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
        "HOME-ALL\n"        # Homing de todos los motores 1-2-3 + status
        "ANGLE1\n"          # Muestra el angulo original del sensor1
        "ANGLE1-STREAM\n"   # Muestra el angulo original del sensor1 continuamente
        "ANGLE2\n"          # Muestra el angulo original del sensor2
        "ANGLE2-STREAM\n"   # Muestra el angulo original del sensor2 continuamente
        "STOP-STREAM\n"     # Detiene la muestra continua del angulo
        "PICK\n"            # Mueve Z hacia abajo, agarra pieza y sube
        "PLACE\n"           # Mueve Z hacia abajo, suelta pieza y sube
        "MOVE\n"            # Mueve motores 1 y 2 a angulos ingresados
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
    # 🔹 2️⃣ Revisar si hay input de teclado sin bloquear
    rlist, _, _ = select.select([sys.stdin], [], [], 0)
    if rlist:
        cmd = sys.stdin.readline().strip()
        print(f"Input: {cmd}")
        commands.send_command(cmd)
        # aqui limpiar el input para que no quede en la consola
        if cmd == "ANGLE1-STREAM" or cmd == "ANGLE2-STREAM":
            streaming = True
            print("\n⚡ Streaming iniciado\n")
        elif cmd == "STOP-STREAM":
            streaming = False
            print("\n⏹ Streaming detenido\n")
        elif cmd == "SHOW-COMMANDS":
            print(
                "Comandos disponibles:\n"
                "ANGLE1\n"
                "ANGLE1-STREAM\n"
                "ANGLE2\n"
                "ANGLE2-STREAM\n"
                "HOME1\n"
                "HOME2\n"
                "HOME3\n"
                "HOME-ALL\n"
                "RESET\n"
                "STATUS\n"
                "STOP-STREAM\n"
                "PICK\n"
                "PLACE\n"
                "MOVE\n"
                "SHOW-COMMANDS\n"
            )
        elif cmd == "MOVE":
            print("Enter the target angle for the shoulder: ")
            shoulder = input().strip()
            print(f"Target angle for the shoulder: {shoulder}")
            print("Enter the angle for the elbow: ")
            elbow = input().strip()
            print(f"Target angle for the elbow: {elbow}")
            commands.send_command(f"MOVE {float(shoulder)} {float(elbow)}")

    time.sleep_ms(10)  # 10 ms → latencia mínima


if __name__ == "__main__":
    main_loop()
