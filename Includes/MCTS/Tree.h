
#pragma once


#include <memory>
#include <vector>
#include <utility>

using std::vector;
using std::pair;

#include "MCTS/Param.h"
#include "MCTS/Node.h"
#include "MCTS/State.h"
#include "MCTS/Evaluator.h"
#include "MCTS/Noise.h"


namespace mcts {
    class MCTS {
    public:
        MCTS(BaseState& init_state, BaseEvaluator& evaluator_, Param& param_=default_param);
        ~MCTS();

        void    Search(int times);
        void    Play(Action action);
        Node*   GetRoot();
        Action  GetOptimalAction();
        void    Reset(BaseState& init_state);
        void    ApplyDirichletNoise(double alpha, double eps);

        friend void DebugLog(MCTS& tree);
    
    private:
        void    ExpandRoot();
        void    SearchSingle();
        void    EvaluateQueue();
        Node*   Select(BaseState* search_state);
        void    Backup(Node* node, Reward z);

        BaseState&      state;
        Param&          param;
        BaseEvaluator&  evaluator;
        Node*           root;
        Dirichet        dirichet_noise;

        vector<pair<Node*, std::unique_ptr<BaseState>>> eval_queue;
    };
}

