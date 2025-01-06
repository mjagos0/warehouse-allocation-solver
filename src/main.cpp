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

struct GlobalConfig {
    bool HEURISTIC = true;
    int RESTARTS = 10;
    int MAXEPOCH = 50000;
    int PATIENCE = 50;
};

struct EvolutionaryAlgorithmConfig {
    int POPULATION_SIZE = 100;
    int TOURNAMENT_SIZE = 5;
};

struct MemeticAlgorithmConfig {
    int POPULATION_SIZE = 100;
    int TOURNAMENT_SIZE = 5;
    float LS_PROBABILITY = 0.5;
    int LS_MAX_DEPTH = 20;
    int LS_PATIENCE = 5;
};

int main() {
    Approach APPROACH = MEMETIC_ALGORITHM;

    ProblemData P(std::cin);
    ProblemStatistics stats;
    std::vector<ProblemSolution> S;

    GlobalConfig gc;
    switch (APPROACH) {
        case LOCAL_SEARCH: {
            S = local_search::solver(P, stats, gc.HEURISTIC, gc.RESTARTS, gc.MAXEPOCH, gc.PATIENCE);
            break;
        }
        case EVOLUTIONARY_ALGORITHM: {
            EvolutionaryAlgorithmConfig ec;
            S = ea::solver(P, stats, gc.HEURISTIC, gc.RESTARTS, gc.MAXEPOCH, gc.PATIENCE, 
                ec.POPULATION_SIZE, ec.TOURNAMENT_SIZE);
            break;
        }
        case MEMETIC_ALGORITHM: {
            MemeticAlgorithmConfig mc;
            S = memetic_algorithm::solver(P, stats, gc.HEURISTIC, gc.RESTARTS, gc.MAXEPOCH, gc.PATIENCE,
                mc.POPULATION_SIZE, mc.TOURNAMENT_SIZE, mc.LS_PROBABILITY, mc.LS_MAX_DEPTH, mc.LS_PATIENCE);
            break;
        }
    }
    
    ProblemStatistics::Run* bestRun = stats.findBestRun();
    processOutput(bestRun);

    return 0;
}

void processOutput(ProblemStatistics::Run* run) {
    std::cout << std::fixed;
    std::cout << "Best solution" <<
        "\nfitness: " << run->lastEpoch()->bestFitness <<
        "\nepoch: " << run->size() <<
        "\nruntime: " << std::to_string(run->lastEpoch()->executionTime.count());
}