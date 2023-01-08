
#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>
#include <vector>
#include <chrono>


using namespace std;


void internal(torch::jit::script::Module& mod, vector<torch::jit::IValue>& inp) {
    chrono::system_clock::time_point start = chrono::system_clock::now();
    torch::jit::IValue out = mod.forward(inp);
    torch::Tensor prob = out.toTuple()->elements()[0].toTensor().to(torch::kCPU);
    torch::Tensor tmp = torch::randn({8, 3, 15, 15}).index({torch::indexing::Slice(), 0}).reshape({8, 225});
    // cout << "prob " << prob.sizes() << ", tmp " << tmp.sizes() << endl;
    torch::Tensor tmp2 = prob * tmp;
    // cout << "tmp2 " << tmp2.sizes() << endl;
    torch::Tensor axsum = torch::sum(tmp2, 1);
    // cout << axsum << endl << axsum.sizes() << endl;
    torch::Tensor tmp3 = tmp2 / axsum.reshape({-1, 1});
    // cout << torch::sum(tmp3, 1) << endl;
    chrono::duration<double> sec = chrono::system_clock::now() - start;
    // cout << out.toTuple()->elements()[0].toTensor().sizes() << endl;
    // cout << out.toTuple()->elements()[1].toTensor().sizes() << endl;
    cout << "time cost " << sec.count() << endl;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: <path-to-exported-script>" << std::endl;
        return -1;
    }

    torch::jit::script::Module model;
    std::cout << "load from " << argv[1] << std::endl;
    try {
        model = torch::jit::load(argv[1]);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model" << std::endl;
        return -1;
    }

    std::cout << "ok" << std::endl;

    // model.to(torch::kCUDA);
    std::cout << "cuda available" << std::endl;

    vector<torch::jit::IValue> inputs;
    // inputs.push_back(torch::randn({8, 5, 15, 15}).to(torch::kCUDA));
    inputs.push_back(torch::randn({8, 5, 15, 15}));
    cout << "input ready" << endl;

    internal(model, inputs);
    internal(model, inputs);

    
    model.eval();
    torch::NoGradGuard no_grad;

    internal(model, inputs);
    internal(model, inputs);
    torch::NoGradGuard no_grad_;
    internal(model, inputs);

    // torch::Tensor prob = out.toTuple()->elements()[0].toTensor();
    // torch::Tensor res = out.toTuple()->elements()[1].toTensor();
    // cout << "prob " << prob.sizes() << endl;
    // cout << "res " << res << endl;
}
