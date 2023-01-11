
#include <iostream>
#include <torch/torch.h>
#include <torch/script.h>

#include "Gomoku/Server.h"


int main(int argc, char *argv[]) {
    gomoku::SelfplayConfig conf;

    if (argc < 2) {
        std::cout << "using default config" << std::endl;
    }
    else {
        std::cout << "load config from " << argv[1] << std::endl;
        conf = gomoku::LoadConfig(argv[1]);
    }

    return gomoku::RunSelfplay(conf);
}
