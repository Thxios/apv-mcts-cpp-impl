
#pragma once


#include <memory>

#include "Param.h"
#include "Node.h"
#include "State.h"
#include "Evaluator.h"


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

		void	Search();
		void	Play(Action action);
		Node*	GetRoot();
		Action	GetOptimalAction();
	
	private:
		// pair<Node*, State&> 	Select();
		Node*	Select(State& search_state);
		void	Backup(Node* node, Reward z);

		State		state;
		Param&      param;
		Evaluator&	evaluator;
		Node*		root;
	};
}

#include "Tree.tpp"
