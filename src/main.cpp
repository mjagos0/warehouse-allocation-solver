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
    ProblemData P(std::cin);
    ProblemSolution S(P);
    ProblemStatistics stats;

    local_search::solver(P, S, stats, true, 10, 10000, 100);
    ProblemStatistics::Run* bestRun = stats.getBestRun();
    processOutput(bestRun);

    return 0;
}

void processOutput(ProblemStatistics::Run* bestRun) {
    std::cout << std::fixed;
    std::cout << "Best solution" <<
        "\nfitness: " << bestRun->lastEpoch()->epochAverage() <<
        "\nepoch: " << bestRun->size() <<
        "\nruntime: " << std::to_string(bestRun->lastEpoch()->executionTime.count());
}