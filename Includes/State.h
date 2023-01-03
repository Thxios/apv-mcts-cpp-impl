
#pragma once

#include <vector>
#include <utility>

using std::vector;
using std::pair;


namespace mcts {

    using Action    = int;
    using Prob      = double;
    using Reward    = double;

    class StateInterface {
    public:
        virtual void    Play(Action action) = 0;
        virtual bool    Terminated()        = 0;
    };

}
