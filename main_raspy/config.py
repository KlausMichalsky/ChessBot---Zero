# ========================================================================
#                 🔹 C H E S S B O T  —   Z E R O 🔹
# ========================================================================
#   Archivo    : config.py
#   Autor      : Klaus Michalsky
#   Fecha      : Feb. 2026
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       - Configuración de pines y parámetros del sistema
#   ▫️ RESPONSABILIDADES
#       - Definir UART y OLED
#       - Centralizar constantes de hardware
# ========================================================================


# Configuración de pines I2C para OLED
OLED_I2C_ID = 0
OLED_I2C_SDA = 4
OLED_I2C_SCL = 5
OLED_FRQ = 400000
# Dimensiones del OLED
OLED_ANCHO = 128
OLED_ALTO = 64
CHAR_ANCHO = 8  # aproximado, según fuente SSD1306
CHAR_ALTO = 16

# Configuración UART
UART_ID = 0
UART_BAUD = 115200
# Pines UART (depende del hardware)
UART_TX_PIN = 0
UART_RX_PIN = 1
