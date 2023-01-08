
#pragma once

#include <iostream>
#include <torch/torch.h>
#include <torch/script.h>

#include "MCTS/Evaluator.h"
#include "Gomoku/Board.h"


namespace gomoku {
    using mcts::Action;
    using mcts::Prob;
    using mcts::BaseState;

    torch::Tensor ToTensor(Board* board);

    class GomokuEvaluator : public mcts::BaseEvaluator {
    public:
        GomokuEvaluator(torch::jit::script::Module& model_, bool noise_=false);

        virtual vector<pair<Reward, vector<pair<Action, Prob>>>>
            EvaluateBatch(vector<BaseState*>& states);
    
    private:
        void WarmUp();
        vector<pair<Reward, vector<pair<Action, Prob>>>> 
            EvaluateBatchInternal(vector<Board*>& states);

        torch::jit::script::Module& model;

        bool    cuda;
        bool    dirichlet_noise;
        double  alpha = 0.1;
        double  epsilon = 0.25;
    };
}

