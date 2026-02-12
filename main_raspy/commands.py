# ========================================================================
#                 🔹 C H E S S B O T  —   Z E R O 🔹
# ========================================================================
#   Archivo    : commands.py
#   Autor      : Klaus Michalsky
#   Fecha      : Feb. 2026
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       - Envío de comandos al RP2040 Zero
#   ▫️ RESPONSABILIDADES
#       - Formatear y enviar comandos
#       - Gestionar respuesta simple
# ========================================================================


import communication


def send_command(cmd):
    communication.write(f"{cmd}\n".encode())
    print("Comando enviado al RP2040 Zero!")
    print("Esperando respuesta...\n")
