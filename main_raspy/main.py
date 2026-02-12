# ========================================================================
#                 🔹 C H E S S B O T  —   Z E R O 🔹
# ========================================================================
#   Archivo    : main.py
#   Autor      : Klaus Michalsky
#   Fecha      : Feb. 2026
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       - Programa principal del Pi
#   ▫️ RESPONSABILIDADES
#       - Pedir comandos
#       - Enviar y recibir datos
#       - Mostrar resultados en consola
# ========================================================================

import time
import commands
import communication


def wait_for_response():

    while True:
        if communication.any():
            msg = communication.readline()
            if msg:
                msg = msg.decode().strip()
                print("RX:", msg)

                if msg == "HOMING_STARTED":
                    print("Secuencia completada.\n")
                    return   # breake sale solo del bucle mas cercano, return sale de la funcion

        time.sleep(0.1)


def main_loop():

    while True:
        command = input("Ingrese comando: ")
        commands.send_command(command)
        wait_for_response()


if __name__ == "__main__":
    main_loop()
