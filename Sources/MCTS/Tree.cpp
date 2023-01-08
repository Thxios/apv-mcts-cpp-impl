
#include "MCTS/Tree.h"


namespace mcts {
    
    MCTS::MCTS(BaseState& init_state, BaseEvaluator& evaluator_, Param& param_)
    : state(init_state), param(param_), evaluator(evaluator_) {
        root = new Node(1.);
    }

    MCTS::~MCTS() {
        delete root;
    }

    Node* MCTS::Select(BaseState* search_state) {
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
    
    void MCTS::Backup(Node* node, Reward z) {
        while (!node->IsRoot()) {
            node->RevertVirtualLoss(param.vloss);
            node->Update(z);

            node = node->parent;
            z = -z;
        }
        node->Update(z);
    }
    
    void MCTS::Search() {
        std::unique_ptr<BaseState> search_state(state.GetCopy());
        Node* node = Select(search_state.get());
        if (search_state->Terminated()) {
            Backup(node, search_state->TerminalReward());
        }
        else {
            eval_queue.emplace_back(node, std::move(search_state));
            if (eval_queue.size() >= param.eval_batch)
                EvaluateQueue();
        }
    }
    
    void MCTS::Search(int times) {
        for (int i = 0; i < times; i++) {
            Search();
        }
        EvaluateQueue();
    }

    void MCTS::EvaluateQueue() {
        if (eval_queue.size() == 0)
            return;

        vector<BaseState*> to_eval;
        for (auto& [_, state_uptr] : eval_queue) {
            to_eval.push_back(state_uptr.get());
        }
        vector<pair<Reward, vector<pair<Action, Prob>>>> evaluated
         = evaluator.EvaluateBatch(to_eval);
        
        for (int i = 0; i < eval_queue.size(); i++) {
            eval_queue[i].first->Expand(evaluated[i].second);
            Backup(eval_queue[i].first, evaluated[i].first);
        }
        eval_queue.clear();
    }

    void MCTS::Play(Action action) {
        Node* new_root = root->LeaveOneChild(action);
        delete root;
        root = new_root;
        state.Play(action);
    }

    Node* MCTS::GetRoot() {
        return root;
    }
    
    Action MCTS::GetOptimalAction() {
        return root->GetOptimalAction();
    }
}

