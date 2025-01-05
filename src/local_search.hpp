#include <vector>

namespace local_search {
    bool terminate(ProblemStatistics::Run& run);
    void LSFirstImproving(ProblemSolution& S);
    ProblemSolution generateNeighbor(ProblemSolution S);

    bool heuristic;
    int epochMax;
    int patienceMax;
    int patience;

    int epoch;
    double lastBestFitness;

    void solve(ProblemData& P, ProblemSolution& S, ProblemStatistics::Run& run) {
        S.randomLayout();
        run.createRecord(epoch, S);
        while (!terminate(run)) {
            std::cout << epoch << " " << lastBestFitness << std::endl;
            LSFirstImproving(S);
            run.createRecord(epoch, S);
        }
    }

    std::vector<ProblemSolution> solver(ProblemData& P, ProblemStatistics& stats,
        bool heuristic = false, int runs = 1, int epochs = 10000, int patience = 10) {
            local_search::heuristic = heuristic;
            local_search::epochMax = epochs;

            std::vector<ProblemSolution> S;
            while (runs--) {    
                local_search::epoch = 0;
                local_search::lastBestFitness = std::numeric_limits<double>::max();
                local_search::patience = patience;
                local_search::patienceMax = patience;

                S.clear();
                S.emplace_back(ProblemSolution(P));

                solve(P, S[0], stats.run);
                stats.saveRun();
            }
            return S;
    }

    bool terminate(ProblemStatistics::Run& run) {
        if (run.lastEpoch()->bestFitness < lastBestFitness) {
            lastBestFitness = run.lastEpoch()->bestFitness;
            patience = patienceMax;
        } else {
            patience--;
        }

        if (epoch++ >= epochMax || patience == 0) {
            return true;
        }
        return false;
    }

    void LSFirstImproving(ProblemSolution& S) {
        ProblemSolution SNeighbor = S;
        SNeighbor.kickRandomCustomer(heuristic);
        if (SNeighbor.fitness < S.fitness)
            S = SNeighbor;
    }
}