
#include "Gomoku/Board.h"


namespace gomoku {

    inline Stone Board::GetColor(Coord pos) {
        return ((board[EMPTY][pos.r][pos.c]) ? EMPTY : 
        (board[BLACK][pos.r][pos.c] ? BLACK : WHITE));
    }

    inline Stone Board::GetColor(Action pos) {
        return GetColor(Action2Coord(pos));
    }

    inline Stone Board::GetColor(int r, int c) {
        return ((board[EMPTY][r][c]) ? EMPTY : 
        (board[BLACK][r][c] ? BLACK : WHITE));
    }

    inline Coord Action2Coord(Action action) {
        return Coord(action / SIZE, action % SIZE);
    }

    inline Action Coord2Action(Coord coord) {
        return coord.r * SIZE + coord.c;
    }

    inline bool Inside(Coord pos) {
        return (0 <= pos.r && pos.r < SIZE) &&
            (0 <= pos.c && pos.c < SIZE);
    }
    inline bool Inside(int r, int c) {
        return (0 <= r && r < SIZE) &&
            (0 <= c && c < SIZE);
    }
}

