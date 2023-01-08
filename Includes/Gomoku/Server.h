
#pragma once

#include <iostream>
#include <chrono>

#include "MCTS/Param.h"
#include "MCTS/Tree.h"
#include "Gomoku/Board.h"
#include "Gomoku/Evaluator.h"


namespace gomoku {
    using namespace mcts;

    class GomokuServer {
    public:
        GomokuServer(const Board& init_state, 
        GomokuEvaluator& evaluator_, mcts::Param& param=mcts::default_param);
        void Run(int computing_budget);

    private:    
        Board   state;
        MCTS    tree;
    };

}

