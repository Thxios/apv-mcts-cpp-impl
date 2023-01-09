
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


void load_test(std::string file_path) {
    std::ifstream in(file_path);

    std::string line;
    int line_idx = 0;
    while (std::getline(in, line)) {
        cout << line_idx << " \"" << line << "\"" << endl;
        line_idx++;

        std::stringstream ss(line);
        string s;
        ss >> s;
        if ((s.size() == 0) || (s[0] == '#')) {
            cout << "invalid line" << endl;
            continue;
        }

        if (s == "MODEL") {
            std::string model_path;
            ss >> model_path;
            if (model_path.size() > 0) {
                cout << "model path \"" << model_path << "\"" << endl;
            }
        }
        else if (s == "N_SEARCH") {
            int n = -1;
            ss >> n;
            if (n > 0) {
                cout << "n " << n << endl;
            }
        }
        else if (s == "SAVE_DIR") {
            std::string save_dir;
            ss >> save_dir;
            if (save_dir.size() > 0) {
                cout << "model path \"" << save_dir << "\"" << endl;
            }
        }
        
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "arg not enougn" << endl;
        return -1;
    }
    load_test(argv[1]);
    // Board bd;
    // vector<int> actions;
    // vector<vector<int>> visit_cnts;

    // for (Coord move: moves) {
    //     bd.Play(Coord2Action(move));
    //     int action = Coord2Action(move);
    //     actions.push_back(action);
    //     vector<int> visit_cnt(SIZE*SIZE, 0);
    //     visit_cnt[action] = 1;
    //     visit_cnts.push_back(visit_cnt);
    //     cout << Coord2Action(move) << " " << move << endl;
        // cout << bd << endl;
        // cout << endl;
    // }
    // cout << bd << endl;
    // cout << "sz: " << SIZE << endl;
    // cout << "state: " << bd.State() << endl;
    // cout << "n_turn: " << bd.turn_elapsed << endl;

    // GomokuLog log(SIZE, bd.State(), bd.turn_elapsed, actions, visit_cnts);
    // log.Save("sample.log");

    // GomokuLog log_loaded("sample.log");
    // Board bd_loaded = log_loaded.ToBoard();
    // cout << bd_loaded << endl;

    cout << BuildFileName(2, "log") << endl;
    std::string s1, s2(""), s3("df");
    cout << s1.empty() << s2.empty() << s3.empty() << endl;
}

