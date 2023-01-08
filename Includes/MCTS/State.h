
#pragma once

#include <memory>


namespace mcts {

    using Action    = int;
    using StateInfo = int;
    using Prob      = double;
    using Reward    = double;
    

    class BaseState {
    public:
        virtual void        Play(Action action) = 0;
        virtual StateInfo   State()             = 0;
        virtual bool        Terminated()        = 0;
        virtual Reward      TerminalReward()    = 0;

        virtual std::unique_ptr<BaseState> GetCopy()   = 0;

        static const StateInfo  ONGOING = 0;
        static const StateInfo  B_WIN   = 1;
        static const StateInfo  W_WIN   = 2;
        static const StateInfo  DRAW    = 3;
    };
}
