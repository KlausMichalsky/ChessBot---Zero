# ========================================================================
#                      🔹P I C O   —   C H E S S🔹
# ========================================================================
#   Archivo    : utils.py
#   Autor      : Klaus Michalsky
#   Fecha      : 2025-12-04
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#       Módulo de utilidades generales para Pico-Chess. Contiene
#       funciones auxiliares reutilizables por otros módulos,
#       ayudando a simplificar y organizar el código.
#   ▫️ RESPONSABILIDADES:
#       - Proveer funciones auxiliares comunes (matemáticas, strings, etc.).
#       - Facilitar la manipulación de datos y estructuras internas.
#       - Reducir la duplicación de código en otros módulos.
#       - Servir como soporte para módulos principales como main.py
#         y chess_engine.py.
# ==========================================================================

def clamp(value, min_val, max_val):
    return max(min_val, min(value, max_val))


def map_value(val, in_min, in_max, out_min, out_max):
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
