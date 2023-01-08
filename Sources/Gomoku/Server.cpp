
#include "Gomoku/Server.h"


namespace mcts {
    void DebugLog(MCTS& tree) {
        std::cout << "root N=" << tree.root->N << std::endl;
        vector<pair<Action, Node*>> children_cp(tree.root->children);
        std::sort(children_cp.begin(), children_cp.end(),
        [](pair<Action, Node*>& a, pair<Action, Node*>& b) {
            return a.second->N > b.second->N;
        });
        for (int i = 0; i < std::min<int>(children_cp.size(), 5); i++) {
            gomoku::Coord p = gomoku::Action2Coord(children_cp[i].first);
            std::cout << p << "  \t";
            std::cout << (*children_cp[i].second) << std::endl;
        }
    }
}

namespace gomoku {

    GomokuServer::GomokuServer(
        const Board& init_state, GomokuEvaluator& evaluator_, mcts::Param& param)
    : state(init_state), tree(state, evaluator_, param) {

    }

    void GomokuServer::Run(int computing_budget) {
        std::cout << "start" << std::endl;
        while (!state.Terminated()) {
            std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

            tree.Search(computing_budget);

            mcts::Action action = tree.GetOptimalAction();
            DebugLog(tree);
            tree.Play(action);

            std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            std::cout << state << std::endl;
            std::cout << std::fixed;
            std::cout << "total time: " << 1000*elapsed.count() << " ms" << std::endl;
            std::cout << std::endl;
        }
        std::cout << "Game Ended" << std::endl;
    }
}
