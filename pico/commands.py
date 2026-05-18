# ========================================================================
#                 🔹 C H E S S B O T  -  Z E R O 🔹
# ========================================================================
#   Archivo    : commands.py
#   Autor      : Klaus Michalsky
#   Fecha      : Feb. 2026
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       - Envío de comandos en formato string simple al RP2040 Zero
#   ▫️ RESPONSABILIDADES
#       - Formatear y enviar comandos al RP2040 Zero
#       - Leer y mostrar respuestas del RP2040 Zero
#       - Gestionar respuesta simple del RP2040 Zero (OK/ERROR)
# ========================================================================


import communication


def send_command(cmd):
    # .encode() convierte el string en bytes.
    communication.write(f"{cmd}\n".encode())
