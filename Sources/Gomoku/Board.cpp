
#include "Gomoku/Board.h"


namespace gomoku {

    Board::Board() {
        Reset();
    }

    Board::~Board() {
        
    }

    void Board::Play(Action action) {
        assert(GetColor(action) == EMPTY);

        Coord pos = Action2Coord(action);
        board[EMPTY][pos.r][pos.c] = 0;
        board[turn][pos.r][pos.c] = 1;

        turn = (BLACK + WHITE) - turn;
        turn_elapsed++;

        last_action = action;
        game_state = CheckState();
    }

    StateInfo Board::State() {
        return game_state;
    }

    bool Board::Terminated() {
        return game_state != ONGOING;
    }

    Reward Board::TerminalReward() {
        assert(Terminated());
        return ((game_state != DRAW) ? 1. : 0.);
    }

    std::unique_ptr<BaseState> Board::GetCopy() {
        return std::unique_ptr<BaseState>(std::make_unique<Board>(*this));
    }

    Board::Iter Board::GetPossibleActions() {
        return Iter(*this);
    }

    void Board::Reset() {
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                board[EMPTY][r][c] = 1;
                board[BLACK][r][c] = 0;
                board[WHITE][r][c] = 0;
            }
        }
        turn = BLACK;
        turn_elapsed = 0;
        game_state = ONGOING;
        last_action = -1;
    }

    StateInfo Board::CheckState() {
        assert(last_action != -1);
        Coord last_coord = Action2Coord(last_action);
        Stone last_color = GetColor(last_action);

        for (int i = 0; i < 4; i++) {
            if (FiveInRow(last_coord, DELTA[i]))
                return ((last_color == BLACK) ? B_WIN : W_WIN);
        }

        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (board[EMPTY][r][c])
                    return ONGOING;
            }
        }

        return DRAW;
    }

    bool Board::FiveInRow(Coord pos, Coord delta) {
        Stone color = GetColor(pos);
        assert(color != EMPTY);
        Coord cur;
        int n_left = 0, n_right = 1;

        cur = pos - delta;
        while (Inside(cur) && board[color][cur.r][cur.c]) {
            cur -= delta;
            n_left++;
        }
        cur = pos + delta;
        while (Inside(cur) && board[color][cur.r][cur.c]) {
            cur += delta;
            n_right++;
        }
        return (n_left + n_right) >= 5;
    }

    Board::Iter& Board::Iter::operator++() {
        do {
            current++;
        }
        while ((current < SIZE * SIZE) && 
            (belong.GetColor(current) != EMPTY));
        if (current >= SIZE * SIZE) {
            current = -1;
        }
        return (*this);
    }

    std::ostream& operator<<(std::ostream& out, Board& b) {
        Coord last_pos;
        if (b.last_action != -1)
            last_pos = Action2Coord(b.last_action);
        
        for (int r = 0; r < SIZE; r++) {
            out << ((r == last_pos.r && last_pos.c == 0) ? '[' : ' ');
            for (int c = 0; c < SIZE; c++) {
                out << (".XO"[b.GetColor(r, c)]);
                if (r == last_pos.r && c == last_pos.c - 1)
                    out << '[';
                else if (r == last_pos.r && c == last_pos.c)
                    out << ']';
                else
                    out << ' ';
            }
            out << '\n';
        }

        out << "TURN: " << b.turn_elapsed << " " << (".XO"[b.turn]) << " - ";

        switch (b.State()) {
        case b.ONGOING:
            out << "ONGOING";
            break;
        case b.B_WIN:
            out << "BLACK(X) WIN";
            break;
        case b.W_WIN:
            out << "WHITE(O) WIN";
            break;
        case b.DRAW:
            out << "DRAW";
            break;
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, Coord &cd){
        out << "(" << cd.r << ", " << cd.c << ")";
        return out;
    }
}

