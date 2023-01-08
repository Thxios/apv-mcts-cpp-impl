
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

using std::vector;

#include "Gomoku/Board.h"


namespace gomoku {
    class GomokuLog {
    public:
        struct LogHeader {
        public:
            LogHeader() : size(0), result(0), len(0) {}
            LogHeader(int size_, int result_, int len_)
            : size(size_), result(result_), len(len_) {}
            uint32_t    size;
            int32_t     result;
            uint32_t    len;
        };

    public:
        GomokuLog(int size, int result, int len, 
            vector<int>& actions, vector<vector<int>>& counts);
        GomokuLog(std::string load_path);
        ~GomokuLog();

        void    Save(std::string save_path);
        Board   ToBoard();

        LogHeader   header;
        int32_t*    actions_ptr;
        int32_t*    counts_ptr;
    };
}

