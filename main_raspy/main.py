# ========================================================================
#                      🔹P I C O   —   C H E S S🔹
# ========================================================================
#   Archivo    : main.py
#   Autor      : Klaus Michalsky
#   Fecha      : 2025-12-04
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       Algoritmo central de Pico-Chess. Gestiona la lógica del juego,
#       toma decisiones de movimiento y controla la interfaz OLED.
#   ▫️ RESPONSABILIDADES:
#       - Calcular movimientos válidos y estrategias del juego.
#       - Enviar instrucciones de movimiento al robot físico (Zero).
#       - Controlar la pantalla OLED con información del estado del juego.
#       - Mantener la lógica y el estado del tablero.
# ========================================================================

from machine import UART, Pin
import time

uart = UART(0, 115200, tx=Pin(0), rx=Pin(1))

command = str(input("Ingrese comando: "))
uart.write(f"{command}\n".encode())

print("Comando enviado al RP2040 Zero!\nEsperando respuesta...\n")


def main_loop():
    # bucle principal
    while True:
        if uart.any():
            msg = uart.readline().decode().strip()
            print(msg)
            if msg == "HOMING_STARTED":
                print("Secuencia completada.")
                break
        time.sleep(0.1)


"""
      # Recibir mensajes
      mensaje = commands.get_command()
       if mensaje is not None:
            # valor_formateado = f"{float(mensaje):.1f}"
            oled.borrar_parte_inferior()
            oled.centrar_texto(1, mensaje)

        # otras tareas repetitivas
        utime.sleep_ms(10)

     # Simulación de calibración de motores
    #     calibrar_motores()
    #     oled.centrar_texto("Motores", CHAR_ALTO)
    #     oled.centrar_texto("Calibrados", 2*CHAR_ALTO)
    #     utime.sleep(3)
    #     oled.mostrar_texto("Sistema listo", "Esperando jugada...")


    # while True:K
    #     # Lógica de ajedrez
    #     movimientos = chess_engine.calcular_siguiente_movimiento()
    #     if movimientos:
    #         x, y = movimientos
    #         uart_zero.enviar_coordenadas(x, y)
    #         posicion_real = uart_zero.recibir_posicion()
    #         oled.mostrar_texto(
    #             f"Moviendo a: {x},{y}", f"Pos real: {posicion_real}")
    #     utime.sleep(0.1)
    """

if __name__ == "__main__":
    main_loop()
