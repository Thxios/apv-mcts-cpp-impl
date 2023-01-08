
#include "MCTS/Tree.h"
#include <iostream>
#include <algorithm>
#include "Gomoku/Board.h"
using namespace gomoku;


namespace mcts {
    
    template <class State, class Evaluator>
    MCTS<State, Evaluator>::MCTS(
        const State& init_state, Evaluator& evaluator_, Param& param_)
    : state(init_state), param(param_), evaluator(evaluator_) {
        root = new Node(1.);
    }

    template <class State, class Evaluator>
    MCTS<State, Evaluator>::~MCTS() {
        delete root;
    }

    template <class State, class Evaluator>
    Node* MCTS<State, Evaluator>::Select(StateInterface* search_state) {
        Node* node = root;
        
        while (!node->IsLeaf()) {
            auto [action, child] = node->Select(param.c_puct);

            node = child;
            node->ApplyVirtualLoss(param.vloss);
            search_state->Play(action);
            if (search_state->Terminated())
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
        node->Update(z);
    }

    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::Search() {
        std::unique_ptr<State> search_state(std::make_unique<State>(state));
        Node* node = Select(static_cast<StateInterface*>(search_state.get()));
        if (search_state->Terminated()) {
            Backup(node, search_state->CurrentReward());
        }
        else {
            eval_queue.emplace_back(node, std::move(search_state));
            // state_queue.push_back(std::move(search_state));
            // node_queue.push_back(node);
            if (eval_queue.size() >= param.eval_batch)
                EvaluateQueue();
        }
    }

    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::Search(int times) {
        for (int i = 0; i < times; i++) {
            Search();
        }
        EvaluateQueue();
    }

    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::EvaluateQueue() {
        if (eval_queue.size() == 0)
            return;

        vector<StateInterface*> to_eval;
        for (auto& [_, state_uptr] : eval_queue) {
            to_eval.push_back(static_cast<StateInterface*>(state_uptr.get()));
        }
        vector<pair<Reward, vector<pair<Action, Prob>>>> evaluated
         = evaluator.EvaluateBatch(to_eval);
        
        for (int i = 0; i < eval_queue.size(); i++) {
            eval_queue[i].first->Expand(evaluated[i].second);
            Backup(eval_queue[i].first, evaluated[i].first);
        }
        // state_queue.clear();
        // node_queue.clear();
        eval_queue.clear();
    }

    /*
    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::Search() {
        vector<Node*> leaves;
        vector<std::unique_ptr<StateInterface>> states;

        for (int i = 0; i < param.eval_batch; i++) {
            // states.push_back(state.GetCopy());
            std::cout << "make unique" << std::endl;
            states.push_back(std::make_unique<State>(state));
            Node* node = Select(
                // dynamic_cast<State*>(states[i].get())
                states[i].get()
            );
            leaves.push_back(node);
        }

        vector<pair<Reward, vector<pair<Action, Prob>>>>
            evaluated = evaluator.EvaluateBatch(states);

        for (int i = 0; i < leaves.size(); i++) {
            leaves[i]->Expand(evaluated[i].second);
            Backup(leaves[i], evaluated[i].first);
        }
    }
    */

    /*
    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::SearchAsync() {
        vector<Node*> leaves;
        vector<std::unique_ptr<StateInterface>> states;

        vector<std::future<Node*>> threads;

        for (int i = 0; i < param.eval_batch; i++) {
            states.push_back(std::make_unique<State>(state));
            threads.push_back(std::async(
                std::launch::async,
                &MCTS<State, Evaluator>::Select,
                this,
                states[i].get()
            ));
        }

        for (int i = 0; i < param.eval_batch; i++) {
            Node* node = threads[i].get();
            leaves.push_back(node);
        }

        vector<pair<Reward, vector<pair<Action, Prob>>>>
            evaluated = evaluator.EvaluateBatch(states);

        for (int i = 0; i < leaves.size(); i++) {
            leaves[i]->Expand(evaluated[i].second);
            Backup(leaves[i], evaluated[i].first);
        }
    }
    */

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

    template <class State, class Evaluator>
    void MCTS<State, Evaluator>::DebugLog() {
        std::cout << "root N=" << root->N << std::endl;
        vector<pair<Action, Node*>> children_cp(root->children);
        std::sort(children_cp.begin(), children_cp.end(),
        [](pair<Action, Node*>& a, pair<Action, Node*>& b) {
            return a.second->N > b.second->N;
        });
        for (int i = 0; i < std::min<int>(children_cp.size(), 5); i++) {
            Coord p = Action2Coord(children_cp[i].first);
            std::cout << children_cp[i].first << " (" << p.r << ", " << p.c << ") ";
            std::cout << (*children_cp[i].second) << std::endl;
        }
    }
}

