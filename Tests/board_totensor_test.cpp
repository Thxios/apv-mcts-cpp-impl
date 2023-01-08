
#include <iostream>
#include <vector>
// #include <torch/torch.h>
// #include <torch/script.h>

#include "Gomoku/Board.h"
#include "Gomoku/Evaluator.h"

using namespace std;
using namespace gomoku;


vector<Coord> moves({
    // Coord(0, 0),
    Coord(5, 5),
    Coord(5, 6),
    Coord(3, 5),
    Coord(4, 4),
    Coord(6, 5),
    Coord(5, 4),
    Coord(7, 5),
    Coord(6, 4),
    Coord(4, 5)
});

int main() {
    Board bd;

    for (Coord move: moves) {
        bd.Play(Coord2Action(move));
        cout << bd << endl;
        cout << endl;
    }
    cout << bd << endl;

    torch::Tensor bd_tensor = ToTensor(&bd);
    cout << bd_tensor << endl;
}


