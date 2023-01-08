
#pragma once


#include <memory>
#include <future>
#include <vector>
#include <utility>

using std::vector;
using std::pair;

#include "MCTS/Param.h"
#include "MCTS/Node.h"
#include "MCTS/State.h"
#include "MCTS/Evaluator.h"


namespace mcts {
    template <class State, class Evaluator>
    class MCTS {

    static_assert(std::is_base_of<StateInterface, State>::value,
        "invalid State type");
    // static_assert(std::is_base_of<EvaluatorInterface<State>, Evaluator>::value,
    //     "invalid Svaluator type");

    public:
        MCTS(const State& init_state, Evaluator& evaluator_, Param& param_=default_param);
        ~MCTS();

        void    Search();
        // void    SearchAsync();
        void    Play(Action action);
        Node*   GetRoot();
        Action  GetOptimalAction();
    
    private:
        Node*   Select(StateInterface* search_state);
        void    Backup(Node* node, Reward z);

        State       state;
        Param&      param;
        Evaluator&  evaluator;
        Node*       root;

        vector<Node*> node_queue;
        vector<std::unique_ptr<StateInterface>> state_queue;
        void    EvaluateQueue();
    };
}

#include "MCTS/Tree.tpp"
