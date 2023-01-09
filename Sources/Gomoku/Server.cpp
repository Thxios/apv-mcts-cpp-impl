
#include "Gomoku/Server.h"


namespace mcts {
    void DebugLog(MCTS& tree) {
        std::cout << "root N=" << tree.root->N << std::endl;
        vector<pair<Action, Node*>> children_cp(tree.root->children);
        std::sort(children_cp.begin(), children_cp.end(),
        [](pair<Action, Node*>& a, pair<Action, Node*>& b) {
            return a.second->N > b.second->N;
        });
        for (int i = 0; i < std::min<int>(children_cp.size(), 5); i++) {
            gomoku::Coord p = gomoku::Action2Coord(children_cp[i].first);
            std::cout << p << "  \t";
            std::cout << (*children_cp[i].second) << std::endl;
        }
    }
}

namespace gomoku {
    GomokuServer::GomokuServer(
        const Board& init_state, GomokuEvaluator& evaluator_, mcts::Param& param)
    : state(init_state), tree(state, evaluator_, param) {

    }

    void GomokuServer::Run(int computing_budget, std::string save_path) {
        vector<Action> action_log;
        vector<vector<int>> count_log;
        bool save_result = !save_path.empty();

        std::cout << "start" << std::endl;
        while (!state.Terminated()) {
            std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

            tree.Search(computing_budget);

            mcts::Action action = tree.GetOptimalAction();

            if (save_result) {
                action_log.push_back(action);
                vector<int> visit_count(SIZE*SIZE, 0);
                for (auto [move, child] : tree.GetRoot()->children) {
                    visit_count[move] = child->N;
                }
                count_log.emplace_back(std::move(visit_count));
            }
            DebugLog(tree);

            tree.Play(action);

            std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            std::cout << state << std::endl;
            std::cout << std::fixed;
            std::cout.precision(2);
            std::cout << "total time: " << 1000*elapsed.count() << " ms" << std::endl;
            std::cout << std::endl;
        }
        std::cout << "Game Ended" << std::endl;

        if (save_result) {
            GomokuLog log(SIZE, state.State(), state.turn_elapsed, action_log, count_log);
            log.Save(save_path);
            std::cout << "Saved in: " << save_path << std::endl;
        }
    }

    void GomokuServer::Reset(const Board& init_state) {
        state = Board(init_state);
        tree.Reset(state);
    }

    int RunSelfplay(SelfplayConfig config) {
        std::cout << "Run Selfplay" << std::endl;
        std::cout << "using config:" << std::endl;
        std::cout << "MODEL\t\t" << config.model_path << std::endl;
        std::cout << "N_SEARCH\t" << config.search_per_move << std::endl;
        std::cout << "SAVE_RESULT\t" << config.save_result << std::endl;
        std::cout << "SAVE_DIR\t" << config.save_dir << std::endl;
        std::cout << "SAVE_START\t" << config.start_no << std::endl;        
        std::cout << "MAX_REPEAT\t" << config.max_repeat << std::endl;
        std::cout << std::endl;

        torch::jit::script::Module model;
        std::cout << "load model from " << config.model_path << std::endl;
        try {
            model = torch::jit::load(config.model_path);
        }
        catch (const c10::Error& e) {
            std::cerr << "error loading the model" << std::endl;
            std::cerr << e.what() << std::endl;
            return -1;
        }

        GomokuEvaluator evaluator(model, true);
        std::cout << "Evaluator ready" << std::endl;

        Param param(3, 5, config.eval_batch);
        Board board;

        GomokuServer server(board, evaluator, param);
        std::cout << "Server ready" << std::endl;

        for (int game = config.start_no; game <= config.max_repeat; game++) {
            std::cout << "===== game " << game << " =====" << std::endl;

            std::string save_path;
            if (config.save_result) {
                save_path += config.save_dir;
                save_path += BuildFileName(game, "bin");
            }

            server.Reset(board);
            server.Run(config.search_per_move, save_path);

            std::cout << std::endl;
        }
        std::cout << "selfplay done" << std::endl;
        return 0;
    }

    SelfplayConfig LoadConfig(std::string config_path) {
        SelfplayConfig conf;
        std::string line;
        std::ifstream in(config_path);
        while (std::getline(in, line)) {
            std::stringstream ss(line);
            std::string s;

            ss >> s;
            if ((s.size() == 0) || (s[0] == '#'))
                continue;

            if (s == "MODEL") {
                std::string model_path;
                ss >> model_path;
                if (model_path.size() > 0)
                    conf.model_path = model_path;
            }
            else if (s == "N_SEARCH") {
                int n = -1;
                ss >> n;
                if (n > 0)
                    conf.search_per_move = n;
            }
            else if (s == "EVAL_BATCH") {
                int n = -1;
                ss >> n;
                if (n > 0)
                    conf.eval_batch = n;
            }
            else if (s == "SAVE_RESULT") {
                int n = -1;
                ss >> n;
                if (n == 1)
                    conf.save_result = true;
            }
            else if (s == "SAVE_DIR") {
                std::string save_dir;
                ss >> save_dir;
                if (save_dir.size() > 0)
                    conf.save_dir = save_dir;
            }
            else if (s == "SAVE_START") {
                int n = -1;
                ss >> n;
                if (n >= 0)
                    conf.start_no = n;
            }
            else if (s == "MAX_REPEAT") {
                int n = -1;
                ss >> n;
                if (n > 0)
                    conf.max_repeat = n;
            }
        }
        if (!conf.save_dir.empty() && 
        (conf.save_dir.back() != '/' || conf.save_dir.back() != '\\')) {
            conf.save_dir += '/';
        }
        return conf;
    }
}
