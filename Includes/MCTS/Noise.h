
#include <random>
#include <vector>


class Dirichet {
public:
    Dirichet() : gen(rd()) {}
    std::vector<double> Sample(double alpha, int size) {
        std::gamma_distribution<double> g(alpha);
        double sum = 0.;
        std::vector<double> prob(size);
        for (int i = 0; i < size; i++) {
            prob[i] = g(gen);
            sum += prob[i];
        }
        for (int i = 0; i < size; i++)
            prob[i] /= sum;
        return prob;
    }

private:
    std::random_device  rd;
    std::mt19937        gen;
};

