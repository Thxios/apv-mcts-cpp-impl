
#include "Gomoku/Evaluator.h"


namespace gomoku {

    torch::Tensor ToTensor(Board* board) {
        torch::Tensor board_plane = torch::from_blob(
            board->board,
            {3, SIZE, SIZE},
            torch::TensorOptions().dtype(torch::kInt32)
        ).to(torch::kFloat32);

        torch::Tensor color_plane;
        if (board->turn == BLACK) {
            color_plane = torch::zeros({1, SIZE, SIZE});
        }
        else {
            color_plane = torch::ones({1, SIZE, SIZE});
            board_plane = board_plane.index({
                torch::tensor({EMPTY, WHITE, BLACK}),
                torch::indexing::Slice(),
                torch::indexing::Slice()
            });
        }

        return torch::cat({
            board_plane,
            color_plane,
            torch::ones({1, SIZE, SIZE})
        });
    }

    GomokuEvaluator::GomokuEvaluator(torch::jit::script::Module& model_, bool noise)
    : model(model_), dirichlet_noise(noise) {
        model.eval();
        cuda = torch::cuda::is_available();
        if (cuda) {
            model.to(torch::kCUDA);
            WarmUp();
        }
    }

    vector<pair<Reward, vector<pair<Action, Prob>>>> 
    GomokuEvaluator::EvaluateBatch(vector<BaseState*>& states) {
        vector<Board*> states_casted;
        for (auto state_ptr : states) {
            states_casted.emplace_back(dynamic_cast<Board*>(state_ptr));
        }
        return EvaluateBatchInternal(states_casted);
    }

    vector<pair<Reward, vector<pair<Action, Prob>>>> 
    GomokuEvaluator::EvaluateBatchInternal(vector<Board*>& states) {
        torch::NoGradGuard();

        vector<torch::Tensor> board_tensor;
        for (Board* state_ptr : states) {
            board_tensor.emplace_back(std::move(ToTensor(state_ptr)));
        }

        torch::Tensor state_tensors = torch::stack(board_tensor);
        if (cuda) {
            state_tensors = state_tensors.to(torch::kCUDA);
        }
        torch::Tensor empty_plane = state_tensors.index({torch::indexing::Slice(), 0}).reshape({-1, SIZE*SIZE});
        vector<torch::jit::IValue> inputs;
        inputs.emplace_back(std::move(state_tensors));

        torch::jit::IValue out = model.forward(inputs);

        torch::Tensor prob_raw = torch::nn::functional::softmax(
            out.toTuple()->elements()[0].toTensor(), 1);
        if (dirichlet_noise) {
            torch::Tensor noise = torch::_sample_dirichlet(
                alpha * torch::ones({(int)states.size(), SIZE*SIZE}));
            if (cuda)
                noise = noise.to(torch::kCUDA);
            prob_raw = prob_raw * (1 - epsilon) + noise * epsilon;
        }
        prob_raw = prob_raw * empty_plane;
        prob_raw = (prob_raw / torch::sum(prob_raw, 1).reshape({-1, 1})).to(torch::kCPU);
        torch::Tensor result = out.toTuple()->elements()[1].toTensor().to(torch::kCPU);
        float* prob_out = prob_raw.data_ptr<float>();
        float* result_out = result.data_ptr<float>();


        vector<pair<Reward, vector<pair<Action, Prob>>>> ret;
        for (int i = 0; i < states.size(); i++) {
            Reward reward = result_out[i];

            vector<pair<Action, Prob>> prob_distribution;
            for (auto iter = states[i]->GetPossibleActions(); iter; ++iter) {
                Action action = iter.GetAction();
                Prob prob = prob_out[i*SIZE*SIZE + action];
                
                prob_distribution.emplace_back(action, prob);
            }
            ret.emplace_back(reward, std::move(prob_distribution));
        }
        return ret;
    }

    void GomokuEvaluator::WarmUp() {
        vector<torch::jit::IValue> inputs;
        inputs.emplace_back(torch::randn({8, N_FEATURES, SIZE, SIZE}).to(torch::kCUDA));
        torch::NoGradGuard no_grad;
        model.forward(inputs);
        model.forward(inputs);
    }
}
