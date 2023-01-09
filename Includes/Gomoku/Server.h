
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <torch/torch.h>
#include <torch/script.h>

#include "MCTS/Param.h"
#include "MCTS/Tree.h"
#include "Gomoku/Board.h"
#include "Gomoku/Evaluator.h"
#include "Gomoku/Logger.h"


namespace gomoku {
    using namespace mcts;

    struct SelfplayConfig {
        int         search_per_move = 0;
        int         eval_batch      = 8;
        bool        save_result     = false;
        int         start_no   = 0;
        int         max_repeat      = 9999;
        std::string model_path;
        std::string save_dir;
    };

    SelfplayConfig  LoadConfig(std::string config_path);
    int             RunSelfplay(SelfplayConfig config);

    class GomokuServer {
    public:
        GomokuServer(const Board& init_state, 
        GomokuEvaluator& evaluator_, mcts::Param& param=mcts::default_param);

        void    Run(int computing_budget, std::string save_path="");
        void    Reset(const Board& init_state);

    private:
        Board   state;
        MCTS    tree;
    };

}

