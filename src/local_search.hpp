#include <vector>

namespace local_search {
    bool terminate(ProblemSolution& S);
    void LSFirstImproving(ProblemSolution& S);
    ProblemSolution generateNeighbor(ProblemSolution S);

    bool heuristic;
    int epochMax;
    int patienceMax;
    int patience;

    int epoch = 0;

    void solve(ProblemData& P, ProblemSolution& S, ProblemStatistics::Run& stats) {
        S.closestLayout();
        while (!terminate(S)) {
            stats.createRecord(epoch, S);
            LSFirstImproving(S);
        }
    }

    void solver(ProblemData& P, ProblemSolution& S, ProblemStatistics& stats,
        bool heuristic = false, int runs = 1, int epochs = 10000, int patience = 10) {
            local_search::heuristic = heuristic;
            local_search::epochMax = epochs;
            while (runs--) {    
                local_search::epoch = 0;
                local_search::patience = patience;
                local_search::patienceMax = patience;
                solve(P, S, stats.run);
                stats.saveRun();
            }
    }

    bool terminate(ProblemSolution& S) {
        static double lastFitness;
        if (lastFitness == S.fitness) {
            patience--;
        } else {
            lastFitness = S.fitness;
            patience = patienceMax;
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