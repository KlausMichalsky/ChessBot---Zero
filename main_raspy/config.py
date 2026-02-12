# ========================================================================
#                      🔹P I C O   —   C H E S S🔹
# ========================================================================
#   Archivo    : config.py
#   Autor      : Klaus Michalsky
#   Fecha      : 2025-12-04
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       Módulo de configuración del proyecto. Contiene constantes
#       y parámetros globales para la pantalla OLED, comunicación
#       UART, motores y demás periféricos.
#   ▫️ RESPONSABILIDADES:
#       - Definir parámetros de hardware como pines y frecuencias.
#       - Mantener constantes de configuración de módulos y librerías.
#       - Servir como fuente central de ajustes para todo el proyecto.
#       - Facilitar la modificación rápida de parámetros sin alterar
#         el resto del código.
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
