# ========================================================================
#                      🔹P I C O   —   C H E S S🔹
# ========================================================================
#   Archivo    : commands.py
#   Autor      : Klaus Michalsky
#   Fecha      : 2025-12-04
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       Módulo encargado de definir y ejecutar los comandos del
#       robot de ajedrez. Gestiona instrucciones recibidas desde
#       el usuario o desde otros módulos.
#   ▫️ RESPONSABILIDADES:
#       - Definir los comandos válidos para el robot.
#       - Interpretar y ejecutar instrucciones recibidas.
#       - Validar entradas y controlar la secuencia de acciones.
#       - Coordinar acciones entre motores, sensores y OLED.
# ========================================================================

import communication


def send_command(cmd):
    communication.write(f"{cmd}\n".encode())
    print("Comando enviado al RP2040 Zero!\nEsperando respuesta...\n")


"""
def get_command():
    if comunication.any():
        msg = comunication.readline()   # devuelve bytes o None
        if msg:                 # evita error por None
            return msg.decode("utf-8").strip()  # type: ignore
    return None


def move_command(x, y):
    return f"MOVE X{x} Y{y}\n"


def set_speed(speed):
    return f"SET SPEED {speed}\n"


def get_pos():
    return "GET POS\n"


def homing():
    return "HOMING\n"


def status():
    return "STATUS?\n"
"""
