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
    Approach APPROACH = LOCAL_SEARCH;
    bool HEURISTIC = true;
    int RESTARTS = 1;
    int MAXEPOCH = 10000;
    int PATIENCE = 50;

    ProblemData P(std::cin);
    ProblemStatistics stats;
    std::vector<ProblemSolution> S;
    switch (APPROACH) {
        case LOCAL_SEARCH:
            S = local_search::solver(P, stats, HEURISTIC, RESTARTS, MAXEPOCH, PATIENCE);
            break;
        case EVOLUTIONARY_ALGORITHM:
            int POPULATION_SIZE = 100;
            int TOURNAMENT_SIZE = 5;
            S = ea::solver(P, stats, HEURISTIC, RESTARTS, MAXEPOCH, PATIENCE);
            break;
        case MEMETIC_ALGORITHM:
            int POPULATION_SIZE = 100;
            int TOURNAMENT_SIZE = 5;
            S = ea::solver(P, stats, HEURISTIC, RESTARTS, MAXEPOCH, PATIENCE);
            break;
    }
    
    ProblemStatistics::Run* bestRun = stats.getBestRun();
    processOutput(bestRun);

    return 0;
}

void processOutput(ProblemStatistics::Run* bestRun) {
    std::cout << std::fixed;
    std::cout << "Best solution" <<
        "\nfitness: " << bestRun->lastEpoch()->bestFitness <<
        "\nepoch: " << bestRun->size() <<
        "\nruntime: " << std::to_string(bestRun->lastEpoch()->executionTime.count());
}