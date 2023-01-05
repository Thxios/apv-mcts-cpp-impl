
#pragma once

#include <utility>
#include <vector>
#include <iostream>
#include <cassert>

using std::vector;
using std::pair;

#include "MCTS/State.h"


namespace gomoku {
    using mcts::Action;
    using mcts::StateInfo;
    using mcts::Reward;

    using Stone	= int;
    class Coord {
    public:
        Coord() : r(-1), c(-1) {}
        Coord(int r_, int c_) : r(r_), c(c_) {}
        int r, c;

        inline Coord operator+ (const Coord& other) {
            return Coord(r + other.r, c + other.c);
        }
        inline Coord operator- (const Coord& other) {
            return Coord(r - other.r, c - other.c);
        }
        inline void operator+= (const Coord& other) {
            r += other.r;
            c += other.c;
        }
        inline void operator-= (const Coord& other) {
            r -= other.r;
            c -= other.c;
        }
        inline Coord operator* (int multiplier) {
            return Coord(multiplier * r, multiplier * c);
        }
    };
    
    static const Coord DELTA[4] = {
        Coord(1, -1),
        Coord(1, 0),
        Coord(1, 1),
        Coord(0, 1),
    };

    const Stone	EMPTY   = 0;
    const Stone	BLACK   = 1;
    const Stone	WHITE   = 2;

    const int SIZE = 15;

    // template <int SIZE>
    class Board : mcts::StateInterface {
    public:
        class Iter {
        public:
            Iter(Board& belong_) : belong(belong_), current(-1) {++(*this);}
            Iter& operator++();
            operator bool() const {return current != -1;}
        private:
            Board& belong;
            Action current;
        };

    public:
        Board();
        virtual ~Board();

        virtual void        Play(Action action);
        virtual StateInfo   State();
        virtual bool        Terminated();
        virtual Reward      CurrentReward();

        void    Reset();
        Iter    GetPossibleActions();

        friend std::ostream& operator<<(std::ostream& out, Board& b);

    private:
        StateInfo CheckState();
        inline Stone GetColor(Coord pos);
        inline Stone GetColor(Action pos);
        inline Stone GetColor(int r, int c);
        bool FiveInRow(Coord pos, Coord delta);

        Stone       turn = BLACK;
        int         turn_elapsed = 0;
        Stone       board[3][SIZE][SIZE];
        StateInfo   game_state = ONGOING;
        Action      last_action = -1;
    };

    inline bool Inside(Coord pos);
    inline bool Inside(int r, int c);
    inline Coord Action2Coord(Action action);
    inline Action Coord2Action(Coord coord);
}


#include "Gomoku/Board.ipp"
