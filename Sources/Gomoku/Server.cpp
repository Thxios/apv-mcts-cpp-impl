
#include "Gomoku/Server.h"


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
            tree.DebugLog();
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
