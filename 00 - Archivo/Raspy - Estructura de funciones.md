
🐍 PICO (MicroPython) – Directorio principal
    /pico/
    │
    ├── main.py
    ├── oled.py
    ├── chess_engine.py
    ├── uart_zero.py
    ├── commands.py
    ├── utils.py
    └── config.py

🟢 1. main.py - El cerebro maestro.
    Responsabilidades:
        Inicializa todo
        Llama al algoritmo de ajedrez
        Envía coordenadas al Zero
        Recibe posición real desde el Zero
        Muestra estado en OLED
    Imports típicos:
        import utime
        import oled
        import uart_zero
        import chess_engine
        from config import *

🟢 2. oled.py - Control total del OLED.
    Responsabilidades:
        Inicializar I2C + pantalla
        Funciones:
        mostrar_texto()
        mostrar_coordenadas()
        mostrar_estado()
        mostrar_error()
        centrar_texto()

🟢 3. chess_engine.py - Lógica del robot de ajedrez
(algoritmo de captura, movimientos especiales, detección de jaque, etc.)
    Responsabilidades:
        Calcular ruta de movimiento
        Generar coordenadas objetivo para el brazo
        Control de secuencia (levantar, mover, bajar)
        Comunicación con main

🟢 4. uart_zero.py - Comunicación Pico ↔ Zero.
    Responsabilidades:
        enviar_coordenadas(x, y)
        recibir_posicion()
        validar mensajes
        parsear datos del Zero

🟢 5. commands.py - Lenguaje de comunicación interno entre Pico ↔ Zero
    Ejemplos:
        MOVE X100 Y200
        SET SPEED 50
        GET POS
        HOMING
        STATUS?
        ERROR?
    Responsabilidades:
        Definir comandos estándar
        Funciones para empaquetarlos

🟢 6. utils.py - Pequeñas herramientas comunes
    Ejemplos:
        mapear valores
        clamp()
        conversión entre grados ↔ pasos
        debounce de botones
        temporizadores

🟢 7. config.py - Parámetros globales
    Incluye:
        Pines OLED
        Baudrate UART pico-zero
        Velocidades de motor esperadas desde el Zero
        Tiempo de movimientos
        Tamaño OLED
        Ejemplo:
        I2C_SDA = 26
        I2C_SCL = 27
        OLED_WIDTH = 128
        OLED_HEIGHT = 64
        UART_ID = 1
        UART_BAUD = 115200