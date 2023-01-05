
#pragma once


#include <memory>
#include <future>

#include "MCTS/Param.h"
#include "MCTS/Node.h"
#include "MCTS/State.h"
#include "MCTS/Evaluator.h"


namespace mcts {
    template <class State, class Evaluator>
    class MCTS {

    static_assert(std::is_base_of<StateInterface, State>::value,
        "invalid State type");
    static_assert(std::is_base_of<EvaluatorInterface<State>, Evaluator>::value,
        "invalid Svaluator type");

    public:
        MCTS(const State& init_state, Param& param_, Evaluator& evaluator_);
        ~MCTS();

        void    Search();
        void    SearchAsync();
        void    Play(Action action);
        Node*   GetRoot();
        Action  GetOptimalAction();
    
    private:
        Node*   Select(State* search_state);
        void    Backup(Node* node, Reward z);

        State       state;
        Param&      param;
        Evaluator&  evaluator;
        Node*       root;
    };
}

#include "MCTS/Tree.tpp"
