#include "warehouse_allocation.hpp"
#include "local_search.hpp"
#include "evolutionary_algorithm.hpp"
#include "memetic_algorithm.hpp"

void processOutput(ProblemStatistics::Run* bestRun);

enum Approach {
    LOCAL_SEARCH,
    EVOLUTIONARY_ALGORITHM,
    MEMETIC_ALGORITHM
};

int main() {
    bool HEURISTIC = true;
    int RESTARTS = 1;
    int MAXEPOCH = 10000;
    int PATIENCE = 50;


    // Local search:
    // ProblemData P(std::cin);
    // ProblemSolution S(P);
    // ProblemStatistics stats;

    // local_search::solver(P, S, stats, HEURISTIC, RESTARTS, MAXEPOCH, PATIENCE);
    // ProblemStatistics::Run* bestRun = stats.getBestRun();
    // processOutput(bestRun);

    // EA:
    int POPULATION = 100;


    ProblemData P(std::cin);
    ProblemStatistics stats;
    std::vector<ProblemSolution> S;
    for (int i = 0; i != POPULATION; i++) {
        std::cout << i << std::endl;
        S.emplace_back(ProblemSolution(P));
    }

    // ea::solver(P, S, stats, HEURISTIC, RESTARTS, MAXEPOCH, PATIENCE);
    

    return 0;
}

void processOutput(ProblemStatistics::Run* bestRun) {
    std::cout << std::fixed;
    std::cout << "Best solution" <<
        "\nfitness: " << bestRun->lastEpoch()->bestFitness <<
        "\nepoch: " << bestRun->size() <<
        "\nruntime: " << std::to_string(bestRun->lastEpoch()->executionTime.count());
}