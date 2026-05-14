# =======================================================================
#             🔹 C H E S S   S Q U A R E   T O   X Y 🔹
# =======================================================================
#  Archivo    : chess_square_to_xy.py
#  Autor      : Klaus Michalsky
#  Fecha      : May-2026
# -----------------------------------------------------------------------
#  ▫️ DESCRIPCIÓN
# -----------------------------------------------------------------------
#  Sistema de conversión de casillas de ajedrez (A1–H8) a coordenadas
#  cartesianas (X, Y) para control de un brazo robótico.
#  Diseñado para integración con sistema de
#  cinemática inversa para el robot KAYRON.
#
#   - Lee casillas desde consola (ej: E2)
#   - Valida formato de casillas de ajedrez
#   - Convierte la casilla a índices de tablero
#   - Calcula coordenadas físicas en mm aplicando offsets del tablero físico
#   - Sirve como base de posicionamiento del efector final
#   - Imprime resultados por consola
#
#  ✅ Funcional
# =======================================================================

import time

# CONSTANTES
# -----------------------------------------------------------------------
# Tamaño de casilla en mm
SQUARE_SIZE = 25.0
# Distancia desde DOF1 a centro Fila A
A1_OFFSET_X = -((3 * SQUARE_SIZE) + SQUARE_SIZE / 2.0)
# Distancia desde DOF1 a centro Rango 1
A1_OFFSET_Y = 80.44


# CONVERSION DE CASILLA XY
# -----------------------------------------------------------------------
def chess_square_to_xy(square):
    # VALIDAR LONGITUD
    # Verifica que el texto tenga exactamente 2 caracteres.
    if len(square) != 2:
        return None

    # OBTENER LETRA Y NÚMERO
    # Obtener la primera letra y convertir a mayúscula
    file = square[0].upper()
    rank = square[1]          # Obtener el número

    # VALIDAR FILA Y RANGO
    if file < 'A' or file > 'H':
        return None

    if rank < '1' or rank > '8':
        return None

    # CONVERTIR A ÍNDICES
    file_index = ord(file) - ord('A')
    rank_index = ord(rank) - ord('1')

    # CONVERTIR A COORDENADAS EN CENTRO DE CASILLAS
    x = A1_OFFSET_X + (file_index * SQUARE_SIZE)
    y = A1_OFFSET_Y + (rank_index * SQUARE_SIZE)

    return x, y


# IMPRIMIR TODAS LAS CASILLAS COMO COORDENADAS XY
# -----------------------------------------------------------------------
def print_board_xy():
    print()
    print("-------- BOARD XY DEBUG --------")

    for file in range(ord('A'), ord('H') + 1):
        for rank in range(ord('1'), ord('8') + 1):

            square = chr(file) + chr(rank)

            x = 0.0
            y = 0.0

            time.sleep(0.1)

            result = chess_square_to_xy(square)

            if result is not None:
                x, y = result

                print(square, end="")
                print(" -> X: ", end="")
                print(x, end="")
                print(" Y: ", end="")
                print(y)

    print("-------- END BOARD --------")
    print()


# MAIN
# -----------------------------------------------------------------------
print("Iniciando...")

# IMPRIMIR TABLERO SOLO 1 VEZ
print_board_xy()

while True:
    square = input("Introduce casilla (ej: E2): ").strip()

    result = chess_square_to_xy(square)

    if result:
        x, y = result
        print(f"{square.upper()} -> X={x} Y={y}")
    else:
        print("Casilla invalida")
