
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <memory>

#include "State.h"
#include "Node.h"
#include "Evaluator.h"
#include "Tree.h"
#include "Param.h"


using namespace std;
using namespace mcts;


const int GAME_LEN = 5;


class TestState : public StateInterface {
public:
    TestState() {}
    TestState(const TestState& other) : moves(other.moves) {
        cout << "cp call " << (*this) << endl;
    }

    virtual ~TestState() {
        cout << "delete " << (*this) << endl;
    }
    virtual void Play(Action action) {
        moves.push_back(action);
    }
    virtual bool Terminated() {
        return moves.size() >= GAME_LEN;
    }
    bool CanMove(Action action) {
        return find(moves.begin(), moves.end(), action) == moves.end();
    }
    friend std::ostream& operator<<(std::ostream& out, TestState& state);

private:
    vector<Action> moves;
};


std::ostream& operator<<(std::ostream& out, TestState& state) {
    out << "State( ";
    for (Action i: state.moves) {
        out << i << " ";
    }
    out << ")";
    return out;
}

template <class State>
class TestEvaluator : public EvaluatorInterface<State> {
public:
    TestEvaluator() : gen(rd()), random_prob(-0.5, 0.5) {}
    virtual ~TestEvaluator() {}

    virtual vector<pair<Reward, vector<pair<Action, Prob>>>>
    EvaluateBatch(vector<unique_ptr<State>>& states) {
        vector<pair<Reward, vector<pair<Action, Prob>>>> evaluated;
        for (auto& state : states) {
            evaluated.push_back(EvaluateSingle(*state));
        }
        return evaluated;
    }

    pair<Reward, vector<pair<Action, Prob>>> EvaluateSingle(State& state) {
        Reward result = random_prob(gen);
        vector<Action> remained_moves;
        for (int i = 0; i < GAME_LEN; i++) {
            if (state.CanMove(i))
                remained_moves.push_back(i);
        }
        vector<pair<Action, Prob>> action_probs;
        for (auto i : remained_moves) {
            action_probs.push_back({i, 1 / (double)remained_moves.size()});
        }
        return {result, action_probs};
    }

private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> random_prob;
};

namespace mcts {
    // template class MCTS<TestState, TestEvaluator<TestState>>;

    void ShowNode(Node* node, int depth, Action action) {
        for (int i = 0; i < depth; i++) 
            cout << "\t";
        cout << (*node);
        if (action >= 0)
            cout << "-A:" << action;
        cout << endl;
        for (auto [a, child] : node->children) {
            ShowNode(child, depth+1, a);
        }
    }
}


int main() {
    cout << "hello, world!" << endl;

    TestState s;
    Param param(3, 5, 3);
    TestEvaluator<TestState> eval;
    MCTS<TestState, TestEvaluator<TestState>> tree(s, param, eval);
    
    cout << endl << "Start Search" << endl;
    for (int i = 0; i < 5; i++) {
        // tree.Search();
        tree.SearchAsync();
        cout << i << " search done" << endl;
    }
    cout << "Search done" << endl;
    ShowNode(tree.GetRoot(), 0, -1);
    Action optimal = tree.GetOptimalAction();
    cout << "Optimal Action: " << optimal << endl << endl;
    tree.Play(optimal);
    cout << "===== after play =====" << endl;
    ShowNode(tree.GetRoot(), 0, -1);
}
