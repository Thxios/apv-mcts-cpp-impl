
#pragma once

#include <memory>
#include <vector>
#include <utility>

using std::vector;
using std::pair;

#include "MCTS/State.h"


namespace mcts
{
    // template <class State>
    class EvaluatorInterface {

    // static_assert(std::is_base_of<StateInterface, State>::value,
    //     "invalid State type");

    public:
        virtual vector<pair<Reward, vector<pair<Action, Prob>>>>
            // EvaluateBatch(vector<std::unique_ptr<State>>& states)   = 0;
            EvaluateBatch(vector<StateInterface*>& states)  = 0;
    };
}

