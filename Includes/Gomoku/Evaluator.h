
#pragma once

#include <memory>
#include <torch/torch.h>
#include <torch/script.h>

#include "MCTS/Evaluator.h"
#include "Gomoku/Board.h"


namespace gomoku {
    using mcts::Action;
    using mcts::Prob;
    using mcts::StateInterface;


    torch::Tensor ToTensor(Board* board);

    class GomokuEvaluator : public mcts::EvaluatorInterface {
    public:
        GomokuEvaluator(torch::jit::script::Module& model_);

        virtual vector<pair<Reward, vector<pair<Action, Prob>>>>
            EvaluateBatch(vector<StateInterface*>& states);
    
    private:
        void WarmUp();
        vector<pair<Reward, vector<pair<Action, Prob>>>> 
            EvaluateBatchInternal(vector<Board*>& states);

        torch::jit::script::Module& model;
        bool    cuda;
    };
}

