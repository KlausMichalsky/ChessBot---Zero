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
import commands
import communication


def wait_for_response(streaming=False):
    """
    ▫️ Función para esperar respuesta del Zero.
    ▫️ Si streaming=True, queda leyendo continuamente
       todos los mensajes que envíe el Zero.
    ▫️ Si streaming=False, solo lee los mensajes disponibles
       y sale de la función.
    """
    while True:
        while communication.any():  # lee todos los datos disponibles
            msg = communication.readline()
            if msg:
                # b'HOMING_STARTED\n' -> "HOMING_STARTED\n" -> "HOMING_STARTED"
                msg = msg.decode().strip()
                print("Status:", msg)

        if not streaming:
            break  # salir si no estamos en modo streaming

        time.sleep(0.1)  # pequeña pausa para evitar 100% CPU


def main_loop():
    """
    ▫️ Bucle principal del programa.
    ▫️ Muestra comandos disponibles.
    ▫️ Envía comandos al Zero.
    ▫️ Gestiona lectura continua de ángulos (streaming) si se activa GET_ANGLE_1_START.
    """
    streaming = False  # Flag para indicar si estamos en modo streaming

    while True:
        if not streaming:
            print(
                "Comandos disponibles:\n"
                "STATUS\n"
                "RESET_ERRORS\n"
                "HOME_MOTOR1\n"
                "HOME_MOTOR2\n"
                "GET_ANGLE_1\n"
                "GET_ANGLE_1_START\n"
                "GET_ANGLE_1_STOP"
            )

            cmd = input("\nIngrese comando: ")
            commands.send_command(cmd)

            # Si es GET_ANGLE_1_START, activamos modo streaming
            if cmd.strip() == "GET_ANGLE_1_START":
                streaming = True
                print(
                    "\n⚡ Streaming de ángulos iniciado. Enviar GET_ANGLE_1_STOP para detener.\n")
                # empieza a leer continuamente
                wait_for_response(streaming=True)
            else:
                wait_for_response(streaming=False)  # lee solo respuesta normal

        else:
            # Estamos en streaming, seguimos leyendo continuamente
            wait_for_response(streaming=True)

            # Chequeamos si el usuario envió GET_ANGLE_1_STOP
            # Para esto, el comando GET_ANGLE_1_STOP debe ser enviado desde la terminal
            # y detectado en el bucle de comunicación. Por simplicidad, hacemos input rápido.
            user_input = input(
                "\nPara detener streaming escriba GET_ANGLE_1_STOP: ")
            if user_input.strip() == "GET_ANGLE_1_STOP":
                commands.send_command(user_input.strip())
                streaming = False
                print("\n⏹ Streaming de ángulos detenido.\n")


if __name__ == "__main__":
    main_loop()
