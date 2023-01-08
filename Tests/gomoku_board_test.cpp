
// g++ -std=c++17 -I Includes/ Tests/gomoku_board_test.cpp Sources/Gomoku/Board.cpp Sources/Gomoku/Logger.cpp

#include <iostream>
#include <vector>

#include "Gomoku/Board.h"
#include "Gomoku/Logger.h"

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
    vector<int> actions;
    vector<vector<int>> visit_cnts;

    for (Coord move: moves) {
        bd.Play(Coord2Action(move));
        int action = Coord2Action(move);
        actions.push_back(action);
        vector<int> visit_cnt(SIZE*SIZE, 0);
        visit_cnt[action] = 1;
        visit_cnts.push_back(visit_cnt);
        cout << Coord2Action(move) << " " << move << endl;
        cout << bd << endl;
        cout << endl;
    }
    cout << bd << endl;
    cout << "sz: " << SIZE << endl;
    cout << "state: " << bd.State() << endl;
    cout << "n_turn: " << bd.turn_elapsed << endl;

    GomokuLog log(SIZE, bd.State(), bd.turn_elapsed, actions, visit_cnts);
    log.Save("sample.log");


    GomokuLog log_loaded("sample.log");
    Board bd_loaded = log_loaded.ToBoard();
    cout << bd_loaded << endl;
}

