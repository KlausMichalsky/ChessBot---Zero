# ========================================================================
#                 🔹 C H E S S B O T  -  Z E R O 🔹
# ========================================================================
#   Archivo    : communication.py
#   Autor      : Klaus Michalsky
#   Fecha      : Feb. 2026
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       - Comunicación UART con el RP2040 Zero
#   ▫️ RESPONSABILIDADES
#       - Inicializar UART
#       - Enviar y recibir datos
# ========================================================================


from machine import UART
import config

# CREACIÓN DE UART
# ------------------------------------------------------------------------
uart = UART(
    config.UART_ID,
    config.UART_BAUD,
    tx=config.UART_TX_PIN,
    rx=config.UART_RX_PIN
)


def write(data):
    uart.write(data)


def any():
    return uart.any()


def readline():
    return uart.readline()

