
// g++ -I Includes -pthread -std=c++17 Tests/thread_test.cpp Sources/MCTS/Node.cpp Sources/Gomoku/Board.cpp 

#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include <vector>
#include <mutex>
#include <queue>
#include <chrono>
#include <cstring>
#include <cassert>

#include "MCTS/State.h"
#include "MCTS/Node.h"
#include "Gomoku/Board.h"

using namespace std;
using namespace gomoku;
using namespace mcts;


vector<Coord> moves({
    Coord(0, 0),
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
bool start = false;


vector<unique_ptr<BaseState>> work_cp(int tidx, Board* repr) {
    while (!start) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    vector<unique_ptr<BaseState>> ret;

    for (int i = 0; i < 10; i++) {
        cout << tidx << endl;
        ret.emplace_back(std::move(repr->GetCopy()));
    }
    return ret;
};

void NodeCopyTest() {

    Board bd;

    for (Coord move: moves) {
        bd.Play(Coord2Action(move));
    }
    cout << bd << endl;
    unique_ptr<Board> ptr = make_unique<Board>(bd);

    vector<future<vector<unique_ptr<BaseState>>>> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(async(work_cp, i, ptr.get()));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    start = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    vector<vector<unique_ptr<BaseState>>> rets;
    for (int i = 0; i < 10; i++) {
        rets.emplace_back(threads[i].get());
    }

    for (int i = 0; i < 10; i++) {
        cout << "from thread " << i << endl;
        for (int j = 0; j < 10; j++) {
            // cout << dynamic_cast<Board&>(*rets[i][j].get()) << endl;
            cout << memcmp(rets[i][j].get(), ptr.get(), sizeof(bd)) << " ";
        }
        cout << endl;
    }
}


// void vloss_work(int tidx, Node* node, condition_variable* cv, mutex* m) {
void vloss_work(int tidx, Node* node) {
    while (!start) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (int i = 0; i < 100; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // cout << tidx << endl;
        node->ApplyVirtualLoss(3);
    }
    for (int i = 0; i < 100; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // cout << -tidx << endl;
        node->RevertVirtualLoss(3);
    }
}

void VLossTest() {
    Node* node = new Node(1.);

    vector<future<void>> threads;
    condition_variable cv;
    mutex m;
    for (int i = 0; i < 10; i++) {
        // threads.emplace_back(async(vloss_work, i, node, &cv, &m));
        threads.emplace_back(async(vloss_work, i, node));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    start = true;
    // cv.notify_all();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cout << (*node) << endl;
    delete node;
}


int main() {
    // NodeCopyTest();
    chrono::system_clock::time_point st = chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    chrono::system_clock::time_point ed = chrono::system_clock::now();
    // VLossTest();

    chrono::duration<double> du, du2 = ed - st;
    chrono::duration<double> du_sum = du + du2;
    du2 = du2.zero();
    cout << du.count() << endl;
    cout << du2.count() << endl;
    cout << du_sum.count() << endl;
}


