
// g++ -std=c++17 -I Includes/ -I Sources/ Tests/gomoku_board_test.cpp Sources/Gomoku/Board.cpp

#include <iostream>
#include <vector>

#include "Gomoku/Board.h"

using namespace std;
using namespace gomoku;


vector<Coord> moves({
    Coord(0, 0),
    Coord(5, 5),
    Coord(5, 6),
    Coord(3, 5),
    Coord(4, 4),
    Coord(6, 5),
    Coord(5, 4),
    Coord(7, 5),
    Coord(6, 4),
    Coord(4, 5)
});

int main() {
    Board bd;

    for (Coord move: moves) {
        bd.Play(Coord2Action(move));
        cout << bd << endl;
        cout << endl;
    }
    cout << bd << endl;
}

