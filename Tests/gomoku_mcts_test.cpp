
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <torch/torch.h>
#include <torch/script.h>

#include "MCTS/Tree.h"
#include "MCTS/Param.h"
#include "Gomoku/Board.h"
#include "Gomoku/Evaluator.h"


using namespace std;
using namespace std::chrono;
using namespace mcts;
using namespace gomoku;


vector<Coord> moves({
    // Coord(0, 0),
    Coord(5, 5),
    Coord(5, 6),
    Coord(3, 5),
    Coord(4, 4),
    Coord(6, 5),
    Coord(5, 4),
    Coord(7, 5),
    Coord(6, 4),
    // Coord(4, 5)
});

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: <path-to-exported-script>" << std::endl;
        return -1;
    }

    torch::jit::script::Module model;
    std::cout << "load from " << argv[1] << std::endl;
    try {
        model = torch::jit::load(argv[1]);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model" << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    GomokuEvaluator evaluator(model);
    cout << "Evaluator ready" << endl;

    Board bd;
    // vector<unique_ptr<Board>> state_copies;
    // vector<StateInterface*> to_eval;

    for (Coord move: moves) {
        bd.Play(Coord2Action(move));
        // unique_ptr<Board> copied(make_unique<Board>(bd));
        // to_eval.push_back(static_cast<StateInterface*>(copied.get()));
        // state_copies.push_back(std::move(copied));
    }
    cout << bd << endl;


    // vector<pair<Reward, vector<pair<Action, Prob>>>> ret = evaluator.EvaluateBatch(to_eval);
    // cout << "done" << endl;
    Param param(3, 5, 8);
    MCTS<Board, GomokuEvaluator> tree(bd, evaluator, param);

    system_clock::time_point start = system_clock::now();
    for (int i = 0; i < 1600; i++) {
        tree.Search();
    }
    tree.EvaluateQueue();
    system_clock::time_point end = system_clock::now();
    duration<double> elapsed = end - start;
    cout << "time cost " << elapsed.count() << endl;
    tree.DebugLog();

}

