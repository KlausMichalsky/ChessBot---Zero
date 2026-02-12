# ========================================================================
#                      🔹P I C O   —   C H E S S🔹
# ========================================================================
#   Archivo    : uart.py
#   Autor      : Klaus Michalsky
#   Fecha      : 2025-12-04
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       Módulo encargado de la comunicación UART entre el Pico y
#       el RP2040-Zero o módulos del robot de ajedrez.
#   ▫️ RESPONSABILIDADES:
#       - Inicializar la interfaz UART y configurar parámetros.
#       - Enviar y recibir datos de manera confiable.
#       - Gestionar protocolos de comunicación internos.
#       - Detectar errores y mantener la comunicación estable.
# ========================================================================

from machine import UART
from config import UART_ID, UART_BAUD, UART_TX_PIN, UART_RX_PIN

uart = UART(UART_ID, UART_BAUD, tx=UART_TX_PIN, rx=UART_RX_PIN)


def any():
    return uart.any()


def readline():
    return uart.readline()


"""
def enviar_angulo_motor(motor_id, angulo):
    message = f"MOTOR_{motor_id}: {angulo}\n"
    uart.write(message)


def enviar_coordenadas(x, y):
    cmd = f"MOVE X{x} Y{y}\n"
    uart.write(cmd)
"""
