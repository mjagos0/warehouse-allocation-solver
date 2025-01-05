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

struct LocalSearch {
    bool HEURISTIC = true;
    int RESTARTS = 10;
    int MAXEPOCH = 50000;
    int PATIENCE = 100;
};

struct EvolutionaryAlgorithmConfig {
    bool HEURISTIC = true;
    int RESTARTS = 1;
    int MAXEPOCH = 10000;
    int PATIENCE = 50;
    int POPULATION_SIZE = 100;
    int TOURNAMENT_SIZE = 5;
};

struct MemeticAlgorithmConfig {
    bool HEURISTIC = true;
    int RESTARTS = 1;
    int MAXEPOCH = 10000;
    int PATIENCE = 50;
    int POPULATION_SIZE = 100;
    int TOURNAMENT_SIZE = 5;
    float LS_PROBABILITY = 0.5;
    int LS_MAX_DEPTH = 20;
    int LS_PATIENCE = 5;
};

int main() {
    Approach APPROACH = LOCAL_SEARCH;
    ProblemData P(std::cin);
    ProblemStatistics stats;
    std::vector<ProblemSolution> S;

    switch (APPROACH) {
        case LOCAL_SEARCH: {
            LocalSearch lc;
            S = local_search::solver(P, stats, lc.HEURISTIC, lc.RESTARTS, lc.MAXEPOCH, lc.PATIENCE);
            break;
        }
        case EVOLUTIONARY_ALGORITHM: {
            EvolutionaryAlgorithmConfig ec;
            S = ea::solver(P, stats, ec.HEURISTIC, ec.RESTARTS, ec.MAXEPOCH, ec.PATIENCE, 
                ec.POPULATION_SIZE, ec.TOURNAMENT_SIZE);
            break;
        }
        case MEMETIC_ALGORITHM: {
            MemeticAlgorithmConfig mc;
            S = memetic_algorithm::solver(P, stats, mc.HEURISTIC, mc.RESTARTS, mc.MAXEPOCH, mc.PATIENCE,
                mc.POPULATION_SIZE, mc.TOURNAMENT_SIZE, mc.LS_PROBABILITY, mc.LS_MAX_DEPTH, mc.LS_PATIENCE);
            break;
        }
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