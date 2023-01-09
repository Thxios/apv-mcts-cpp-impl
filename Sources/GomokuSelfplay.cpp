
#include <iostream>
#include <torch/torch.h>
#include <torch/script.h>
using namespace std;

#include "Gomoku/Server.h"


int main(int argc, char *argv[]) {
    gomoku::SelfplayConfig conf;

    if (argc < 2) {
        cout << "using default config" << endl;
    }
    else {
        cout << "load config from " << argv[1] << endl;
        conf = gomoku::LoadConfig(argv[1]);
    }

    return gomoku::RunSelfplay(conf);
}
