
#pragma once


namespace mcts {

    class Param {
    public:
        Param(
            int     vloss_      = 3,
            double  c_puct_     = 5,
            int     eval_batch_ = 1
        ) 
        : vloss(vloss_), c_puct(c_puct_), eval_batch(eval_batch_) {}

        const int       vloss;
        const double    c_puct;
        const int       eval_batch;
    };

    const Param default_param;
}
