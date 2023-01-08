
#include "Gomoku/Logger.h"


namespace gomoku {
    GomokuLog::GomokuLog(int size, int result, 
    int len, vector<int>& actions, vector<vector<int>>& counts)
    : header(size, result, len) {
        actions_ptr = new int32_t[len];
        counts_ptr = new int32_t[len * size*size];

        for (int t = 0; t < len; t++) {
            actions_ptr[t] = actions[t];
            for (int action = 0; action < size*size; action++) {
                counts_ptr[t*size*size + action] = counts[t][action];
            }
        }
    }

    GomokuLog::GomokuLog(std::string load_path) {
        std::ifstream in;
        in.open(load_path, std::ios::in | std::ios::binary);
        assert(in.is_open());

        in.read((char*)(&header), sizeof(header));
        actions_ptr = new int32_t[header.len];
        counts_ptr = new int32_t[header.len*header.size*header.size];
        in.read((char*)actions_ptr, sizeof(int32_t) * header.len);
        in.read((char*)counts_ptr, sizeof(int32_t) * header.len*header.size*header.size);

        in.close();
    }

    GomokuLog::~GomokuLog() {
        delete[] actions_ptr;
        delete[] counts_ptr;
    }
    
    void GomokuLog::Save(std::string save_path) {
        std::ofstream out;
        out.open(save_path, std::ios::out | std::ios::binary);
        assert(out.is_open());

        out.write((char*)(&header), sizeof(header));
        out.write((char*)actions_ptr, sizeof(int32_t) * header.len);
        out.write((char*)counts_ptr, sizeof(int32_t) * header.len*header.size*header.size);

        out.close();
    }

    Board GomokuLog::ToBoard() {
        assert(SIZE == header.size);
        Board board;

        for (int i = 0; i < header.len; i++) {
            board.Play(actions_ptr[i]);
        }

        assert(board.State() == header.result);
        return board;
    }
}
