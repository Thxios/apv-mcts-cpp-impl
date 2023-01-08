
#pragma once

#include <memory>
#include <vector>
#include <utility>

using std::vector;
using std::pair;

#include "MCTS/State.h"


namespace mcts
{
    class BaseEvaluator {
    public:
        virtual ~BaseEvaluator() {};

        virtual vector<pair<Reward, vector<pair<Action, Prob>>>>
            EvaluateBatch(vector<BaseState*>& states)  = 0;
    };
}

