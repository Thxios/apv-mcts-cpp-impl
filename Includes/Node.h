
#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <memory>
#include <iostream>

using std::vector;
using std::pair;

#include "State.h"


namespace mcts {
    class Node;
    using NodePtr = std::shared_ptr<Node>;

    class Node {
    public:
        Node(Prob prob, Node* parent_=nullptr);
        ~Node();
    
        void				Expand(vector<pair<Action, Prob>>& prob_distribution);
        pair<Action, Node*>	Select(double c_puct);
        void				Update(Reward z);
        void				ApplyVirtualLoss(int vloss);
        void				RevertVirtualLoss(int vloss);
        Node*				LeaveOneChild(Action action);
        Action				GetOptimalAction();

        bool    IsRoot();
        bool    IsLeaf();
        Reward  Q();
        double  U();
        int     Count();

        Node*   parent;
        
        friend std::ostream& operator<<(std::ostream& out, Node& node);
        friend void ShowNode(Node* node, int depth, Action action);

    private:
        Prob	P;
		Reward	W		= 0.;
        int		N		= 0;
        double	N_sqrt	= 0.;

        vector<pair<Action, Node*>> children;
    };

}
