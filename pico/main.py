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


# FUNCIÓN PRINCIPAL DEL SISTEMA
# Inicializa comandos disponibles y ejecuta el bucle principal.
# -----------------------------------------------------------------------
def main_loop():
    global streaming
    global shoulder, elbow

    print(
        "Comandos disponibles:\n"
        "STATUS\n"  # Mostrar el estado actual del sistema
        "RESET\n"  # Reiniciar el sistema
        "ANGLES\n"  # Mostrar los ángulos actuales del brazo
        "BOARD\n"  # Mostrar el tablero
        "COMMANDS\n"  # Mostrar esta lista de comandos
        "HOME\n"  # Mover el brazo a la posición de inicio general
        "HOMING\n"  # Hacer Homing para calibrar el sistema
        "MOVE\n"  # Mover el brazo de una casilla a otra
        # "HOME1\n"
        # "HOME2\n"
        # "HOME3\n"
        # "PICK\n"  # Recoger una pieza
        # "PLACE\n"  # Colocar una pieza
        # "SQUARE\n"
    )

    print("Command: ")

    # BUCLE PRINCIPAL INFINITO:
    # Lee mensajes UART continuamente y revisa input del teclado.
    # -----------------------------------------------------------------------
    while True:
        if communication.any():
            msg = communication.readline()
            if msg:
                clean_msg = msg.decode('utf-8', 'ignore').rstrip('\r\n')
                print(clean_msg)

        keyboard_input()


# LECTURA DE MENSAJES UART DISPONIBLES
# Los decodifica y los imprime limpios.
# -----------------------------------------------------------------------
def readUart():
    if communication.any():
        msg = communication.readline()
        if msg:
            clean_msg = msg.decode('utf-8', 'ignore').rstrip('\r\n')
            print(clean_msg)


# GESTIÓN DE COMANDOS DESDE TECLADO
# Lee, valida y ejecuta comandos ingresados por el usuario.
# DIVISIÓN DE COMANDOS DEL SISTEMA
# Comandos especiales (MOVE, SQUARE): requieren inputs adicionales del usuario.
# Comandos simples (HOME, RESET, PICK, etc.): se envían directamente sin procesamiento.
# -----------------------------------------------------------------------
def keyboard_input():
    global streaming

    rlist, _, _ = select.select([sys.stdin], [], [], 0)

    if not rlist:
        return

    cmd = sys.stdin.readline().strip().upper()
    print(f"Input: {cmd}")

    # VALIDACIÓN DE CAPTURA EN COMANDO MOVE
    # Solicita al usuario una respuesta válida Y o N.
    # -----------------------------------------------------------------------
    if cmd == "MOVE":

        while True:
            capture = input("Capture? (Y/N): ").strip().upper()
            # esta el input del usuar que se guarda en capture dentro de la lista de opciones validas?
            if capture in ["Y", "N"]:
                break
            print("Invalid value. Enter Y or N.")

        # VALIDACIÓN DE CASILLA INICIAL (A1-H8)
        # Repite hasta que el usuario ingrese una coordenada de ajedrez válida.
        # -----------------------------------------------------------------------
        while True:
            start_square = input("Start square: ").strip().upper()

            if (
                len(start_square) != 2
                or start_square[0] < 'A'
                or start_square[0] > 'H'
                or start_square[1] < '1'
                or start_square[1] > '8'
            ):
                print(f"Invalid start square: {start_square}")
                continue
            break

        # VALIDACIÓN DE CASILLA FINAL (A1-H8) Y ENVÍO DE COMANDO MOVE/CAPTURE
        # Construye y envía el comando según si hay captura o no.
        # -----------------------------------------------------------------------
        while True:
            end_square = input("End square: ").strip().upper()

            if (
                len(end_square) != 2
                or end_square[0] < 'A'
                or end_square[0] > 'H'
                or end_square[1] < '1'
                or end_square[1] > '8'
            ):
                print(f"Invalid end square: {end_square}")
                continue
            break

        if capture == "Y":
            commands.send_command(f"CAPTURE {start_square} {end_square}")
        else:
            commands.send_command(f"MOVE {start_square} {end_square}")
        return

    # # ❌ Este bloque se puede borrar en  futuras versiones
    # -----------------------------------------------------------------------
    # elif cmd == "SQUARE":

    #     while True:
    #         square = input("Enter square (example E4): ").strip().upper()

    #         if len(square) != 2:
    #             print("Invalid square length.")
    #             continue

    #         file = square[0]
    #         rank = square[1]

    #         if file < 'A' or file > 'H':
    #             print("Invalid file. Use A-H.")
    #             continue

    #         if rank < '1' or rank > '8':
    #             print("Invalid rank. Use 1-8.")
    #             continue

    #         break

    #     commands.send_command(f"SQUARE {square}")
    #     return

    # MUESTRA COMANDOS DISPONIBLES
    # -----------------------------------------------------------------------
    elif cmd == "SHOW-COMMANDS":
        print(
            "Comandos disponibles:\n"
            "STATUS\n"
            "RESET\n"
            "ANGLES\n"
            "BOARD\n"
            "COMMANDS\n"
            "HOME\n"
            "HOMING\n"
            "MOVE\n"
            # "HOME1\n"
            # "HOME2\n"
            # "HOME3\n"
            # "PICK\n"
            # "PLACE\n"
            # "SQUARE\n"
        )
        return

    # MODO DIRECTO: todo comando no especial se envía al robot sin procesamiento.
    # -----------------------------------------------------------------------
    else:
        commands.send_command(cmd)
        return


if __name__ == "__main__":
    main_loop()
