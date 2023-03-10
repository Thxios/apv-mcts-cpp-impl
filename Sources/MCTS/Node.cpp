
#include "MCTS/Node.h"


namespace mcts {
    Node::Node(Prob prob, Node* parent_)
        : P(prob), parent(parent_) {}

    Node::~Node() {
        // std::cout << "delete " << (*this) << std::endl;
        for (auto [_, child] : children) {
            delete child;
        }
    }

    void Node::Expand(vector<pair<Action, Prob>>& prob_distribution) {
        if (!children.empty())
            return;

        for (auto [action, prob] : prob_distribution) {
            children.emplace_back(action, new Node(prob, this));
        }
    }

    pair<Action, Node*> Node::Select(double c_puct) {
        auto puct_comp = [&](pair<Action, Node*>& a, pair<Action, Node*>& b) {
            return a.second->UCT(c_puct) < b.second->UCT(c_puct);
        };

        return *std::max_element(children.begin(), children.end(), puct_comp);
    }

    void Node::ApplyVirtualLoss(int vloss) {
        N += vloss;
        W -= vloss;
    }

    void Node::RevertVirtualLoss(int vloss) {
        N -= vloss;
        W += vloss;
    }

    void Node::Update(Reward z) {
        N++;
        N_sqrt = sqrt(N);
        W += z;
    }

    Node* Node::LeaveOneChild(Action action) {
        auto comp = [=](pair<Action, Node*>& action_node) -> bool {
            return action_node.first == action;
        };

        auto action_node_iter = std::find_if(children.begin(), children.end(), comp);
        assert(action_node_iter != children.end());

        Node *node = action_node_iter->second;
        node->parent = nullptr;
        children.erase(action_node_iter);
        return node;
    }

    Action Node::GetOptimalAction() {
        auto visit_comp = [](pair<Action, Node*>& a, pair<Action, Node*>& b) {
            return a.second->N < b.second->N;
        };
        return (*std::max_element(children.begin(), children.end(), visit_comp)).first;
    }

    bool Node::IsRoot() {
        return !parent;
    }
    
    bool Node::IsLeaf() {
        return children.empty();
    }

    Reward Node::Q() {
        if (N == 0)
            return 0;
        return W / (double)N;
    }

    double Node::UCT(double c_puct) {
        return Q() + c_puct * parent->N_sqrt * P / (double)(1 + N);
    }
    
    std::ostream& operator<<(std::ostream& out, Node& node) {
        out << std::fixed;
        out.precision(3);
        out << "Node(";
        out << "P=" << node.P;
        out << " Q=" << node.Q();
        out << " N=" << node.N;
        if (node.parent)
            out << " UCT=" << node.UCT(5);
        out << ")";
        return out;
    }
}

