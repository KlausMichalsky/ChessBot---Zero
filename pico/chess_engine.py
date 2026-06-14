# ========================================================================
#                      🔹P I C O   —   C H E S S🔹
# ========================================================================
#   Archivo    : chess_engine.py
#   Autor      : Klaus Michalsky
#   Fecha      : 2025-12-04
# ------------------------------------------------------------------------
#   ▫️ DESCRIPCIÓN
#   Este módulo implementa un motor de ajedrez utilizando la biblioteca
#   `python-chess` y el motor de ajedrez Stockfish.
# ========================================================================

import chess
import chess.engine

engine = chess.engine.SimpleEngine.popen_uci("/usr/games/stockfish")

board = chess.Board()


def jugar(movida):
    board.push_san(movida)

    result = engine.play(board, chess.engine.Limit(time=0.1))
    print("Stockfish juega:", result.move)

    board.push(result.move)

    return result.move


while True:
    entrada = input("Tu jugada (ej: e2e4): ")
    jugar(entrada)
