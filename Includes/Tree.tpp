
#pragma once
#include "Tree.h"


namespace mcts {
    
    template <class State, class Evaluator>
    MCTS<State, Evaluator>::MCTS(
        const State& init_state, Param& param_, Evaluator& evaluator_)
    : state(init_state), param(param_), evaluator(evaluator_) {
        root = new Node(1.);
    }

    template <class State, class Evaluator>
    MCTS<State, Evaluator>::~MCTS() {
        delete root;
    }

    template <class State, class Evaluator>
    Node* MCTS<State, Evaluator>::Select(State& search_state) {
        Node* node = root;
        
        while (!node->IsLeaf()) {
            auto [action, child] = node->Select(param.c_puct);

            node = child;
            node->ApplyVirtualLoss(param.vloss);
            search_state.Play(action);
            if (search_state.Terminated())
                break;
        }
        
        return node;
    }

    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::Backup(Node* node, Reward z) {
        while (!node->IsRoot()) {
            node->RevertVirtualLoss(param.vloss);
            node->Update(z);

            node = node->parent;
            z = -z;
        }
    }

    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::Search() {
        vector<Node*> leaves;
        vector<std::unique_ptr<State>> states;

        for (int i = 0; i < param.eval_batch; i++) {
            std::unique_ptr<State> copied(std::make_unique<State>(state));
            
            Node* node = Select(*copied);

            leaves.push_back(node);
            states.push_back(std::move(copied));
        }

        vector<pair<Reward, vector<pair<Action, Prob>>>>
            evaluated = evaluator.EvaluateBatch(states);

        for (int i = 0; i < leaves.size(); i++) {
            leaves[i]->Expand(evaluated[i].second);
            Backup(leaves[i], evaluated[i].first);
        }
    }

    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::Play(Action action) {
        Node* new_root = root->LeaveOneChild(action);
        delete root;
        root = new_root;
        state.Play(action);
    }

    template <class State, class Evaluator>
    Node* MCTS<State, Evaluator>::GetRoot() {
        return root;
    }

    template <class State, class Evaluator>
    Action MCTS<State, Evaluator>::GetOptimalAction() {
        return root->GetOptimalAction();
    }
}

