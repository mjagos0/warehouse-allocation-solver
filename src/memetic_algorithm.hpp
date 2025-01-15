#ifndef MEMETIC_ALGORITHM_HPP
#define MEMETIC_ALGORITHM_HPP

#include <vector>
#include "local_search.hpp"

namespace memetic_algorithm {
    bool terminate(ProblemStatistics::Run& run);
    ProblemSolution generateNeighbor(ProblemSolution S);
    void initPopulation(std::vector<ProblemSolution>& S);
    std::vector<ProblemSolution*> tournamentSelection(std::vector<ProblemSolution>& S, int N, int M);
    ProblemSolution onePointCrossover(ProblemSolution* S1, ProblemSolution* S2);
    std::vector<ProblemSolution> generateOffspring(std::vector<ProblemSolution>& S);
    std::vector<ProblemSolution> generationalReplacement(std::vector<ProblemSolution>& P, std::vector<ProblemSolution>& O);
    void LocalImprovement(std::vector<ProblemSolution>& S);

    bool heuristic;
    int epochMax;
    int patienceMax;
    int patience;

    int populationSize;
    int tournamentSize;
    float LSProbability;
    int LSMaxDepth;
    int LSPatience;

    int epoch;
    double lastBestFitness;

    void solve(ProblemData& P, std::vector<ProblemSolution>& S, ProblemStatistics::Run& run) {
        initPopulation(S);
        run.createRecord(epoch, S);
        while (!terminate(run)) {
            std::cout << epoch << "\t" << lastBestFitness << std::endl;
            std::vector<ProblemSolution> O = generateOffspring(S);
            LocalImprovement(O);
            S = generationalReplacement(S, O);
            run.createRecord(epoch, S);
        }
    }

    std::vector<ProblemSolution> solver(ProblemData& P, ProblemStatistics& stats,
        bool heuristic = false, int runs = 1, int epochs = 10000, int patience = 10,
        int populationSize = 100, int tournamentSize = 5,
        float LSProbability = 0.5, int LSMaxDepth = 50, int LSPatience = 10) {
            memetic_algorithm::heuristic = heuristic;
            memetic_algorithm::epochMax = epochs;
            memetic_algorithm::populationSize = populationSize;
            memetic_algorithm::tournamentSize = tournamentSize;
            memetic_algorithm::LSProbability = LSProbability;
            memetic_algorithm::LSMaxDepth = LSMaxDepth;
            memetic_algorithm::LSPatience = LSPatience;
            
            std::vector<ProblemSolution> S;
            S.reserve(populationSize);
            while (runs--) {    
                memetic_algorithm::epoch = 0;
                memetic_algorithm::lastBestFitness = std::numeric_limits<double>::max();
                memetic_algorithm::patience = patience;
                memetic_algorithm::patienceMax = patience;

                S.clear();
                for (int i = 0; i != memetic_algorithm::populationSize; i++) {
                    S.emplace_back(ProblemSolution(P));
                }

                solve(P, S, stats.run);
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

    void initPopulation(std::vector<ProblemSolution>& S) {
        for (ProblemSolution& s : S) {
            s.randomLayout();
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
            if (!O.isValidSolution() || !O.checkAllocation()) {
                std::runtime_error("Invalid offspring generated");
            }
            offspring.push_back(O);
        }
        return offspring;
    }

    std::vector<ProblemSolution> generationalReplacement(std::vector<ProblemSolution>& P, std::vector<ProblemSolution>& O) {
        return O;
    }

    void LocalImprovement(std::vector<ProblemSolution>& S) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        int patience = memetic_algorithm::LSPatience;
        double lastFitness = std::numeric_limits<double>::max();

        for (ProblemSolution& s : S) {
            float number = dist(gen);
            if (number < memetic_algorithm::LSProbability) {
                for (int i = 0; i != LSMaxDepth; i++) {
                    local_search::LSFirstImprovingModif(s);
                    if (s.fitness != lastFitness) {
                        int patience = memetic_algorithm::LSPatience;
                    } else if (!--patience) {
                        continue;
                    }
                }
            }
        }
    }
}
#endif
