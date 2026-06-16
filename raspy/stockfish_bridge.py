"""
ZERO---CHESS
----------------------------------------
Comunicación con Stockfish mediante UCI
Raspberry Pi Zero 2 W
----------------------------------------
"""

import chess
import chess.engine


class StockfishBridge:

    def __init__(self,
                 engine_path="/usr/games/stockfish",
                 think_time=0.1):

        self.engine = chess.engine.SimpleEngine.popen_uci(engine_path)
        self.board = chess.Board()
        self.think_time = think_time

    def new_game(self):
        self.board.reset()

    def push_move(self, move_uci):
        """
        move_uci ejemplo:
        e2e4
        g1f3
        """

        move = chess.Move.from_uci(move_uci)

        if move in self.board.legal_moves:
            self.board.push(move)
            return True

        return False

    def get_best_move(self):

        result = self.engine.play(
            self.board,
            chess.engine.Limit(time=self.think_time)
        )

        best_move = result.move.uci()

        self.board.push(result.move)

        return best_move

    def get_fen(self):
        return self.board.fen()

    def close(self):
        self.engine.quit()


# ==========================================================
# TEST
# ==========================================================

if __name__ == "__main__":

    stockfish = StockfishBridge()

    stockfish.push_move("e2e4")

    reply = stockfish.get_best_move()

    print("Stockfish:", reply)

    stockfish.close()
