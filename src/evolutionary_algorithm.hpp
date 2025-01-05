#include <vector>

namespace ea {
    bool terminate(ProblemStatistics::Run& run);
    ProblemSolution generateNeighbor(ProblemSolution S);
    void initPopulation(std::vector<ProblemSolution>& S);
    std::vector<ProblemSolution*> tournamentSelection(std::vector<ProblemSolution>& S, int N, int M);
    ProblemSolution onePointCrossover(ProblemSolution* S1, ProblemSolution* S2);
    std::vector<ProblemSolution> generateOffspring(std::vector<ProblemSolution>& S);
    std::vector<ProblemSolution> generationalReplacement(std::vector<ProblemSolution>& P, std::vector<ProblemSolution>& O);

    bool heuristic;
    int epochMax;
    int patienceMax;
    int patience;

    int populationSize;
    int tournamentSize;
    int tournamentWinners;

    int epoch;
    double lastBestFitness;

    void solve(ProblemData& P, std::vector<ProblemSolution>& S, ProblemStatistics::Run& run) {
        initPopulation(S);
        while (!terminate(run)) {
            run.createRecord(epoch, S);
            std::vector<ProblemSolution> O = generateOffspring(S);
            generationalReplacement(S, O);
        }
    }

    void solver(ProblemData& P, std::vector<ProblemSolution>& S, ProblemStatistics& stats,
        bool heuristic = false, int runs = 1, int epochs = 10000, int patience = 10) {
            ea::heuristic = heuristic;
            ea::epochMax = epochs;
            ea::populationSize = S.size();
            ea::tournamentSize = 5;
            ea::tournamentWinners = 2;
            
            while (runs--) {    
                ea::epoch = 0;
                ea::lastBestFitness = std::numeric_limits<double>::max();
                ea::patience = patience;
                ea::patienceMax = patience;
                solve(P, S, stats.run);
                stats.saveRun();
            }
    }

    bool terminate(ProblemStatistics::Run& run) {
        if (run.lastEpoch()->bestFitness < lastBestFitness) {
            patience--;
        } else {
            lastBestFitness = run.lastEpoch()->bestFitness;
            patience = patienceMax;
        }

        if (epoch++ >= epochMax || patience == 0) {
            return true;
        }
        return false;
    }

    void initPopulation(std::vector<ProblemSolution>& S) {
        for (ProblemSolution& s : S) {
            s.closestLayout();
        }
    }

    std::vector<ProblemSolution*> tournamentSelection(std::vector<ProblemSolution>& S, int N, int M) {
        static std::mt19937 rng(std::random_device{}());
        std::vector<ProblemSolution*> winners;
        std::vector<ProblemSolution*> pool;

        for (ProblemSolution& solution : S) {
            pool.push_back(&solution);
        }

        for (int m = 0; m != M; m++) {
            if (pool.size() < N) {
                throw std::runtime_error("Not enough unique solutions for the next tournament.");
            }
            std::shuffle(pool.begin(), pool.end(), rng);
            std::vector<ProblemSolution*> participants(pool.begin(), pool.begin() + N);

            double bestFitness = std::numeric_limits<double>::max();
            ProblemSolution* winner = nullptr;
            for (ProblemSolution* p : participants) {
                if (p->fitness < bestFitness) {
                    bestFitness = p->fitness;
                    winner = p;
                }
            }
            winners.push_back(winner);
            pool.erase(std::remove(pool.begin(), pool.end(), winner), pool.end());
        }
        return winners;
    }

    ProblemSolution onePointCrossover(ProblemSolution* S1, ProblemSolution* S2) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(0, S1->P->M - 1);

        int cutoff = dist(gen);
        ProblemSolution O(*(S1->P));
        for (int c = 0; c != S1->P->M; c++) {
            if (c < cutoff) {
                O.allocateWarehouse(c, S1->W_C[c]);
            } else {
                O.allocateWarehouse(c, S2->W_C[c]);
            }
        }

        for (int w = 0; w != O.P->N; w++) {
            while (O.CAP_A_W[w] < 0) {
                int c = *(O.C_W[w].begin());
                int w_ = O.findClosestAvailableWarehouse(c, w);
                if (w_ != -1) {
                    O.allocateWarehouse(c, w_);
                } else {
                    O.resolveHangingCustomer(c);
                }
            }
        }
        return O;
    }

    std::vector<ProblemSolution> generateOffspring(std::vector<ProblemSolution>& S) {
        std::vector<ProblemSolution> offspring;
        offspring.reserve(populationSize);

        for (int i = 0; i < populationSize; ++i) {
            std::vector<ProblemSolution*> parents = tournamentSelection(S, tournamentSize, 2);
            ProblemSolution O = onePointCrossover(parents[0], parents[1]);
            offspring.push_back(O);
        }
        return offspring;
    }

    std::vector<ProblemSolution> generationalReplacement(std::vector<ProblemSolution>& P, std::vector<ProblemSolution>& O) {
        return O;
    }
}