#ifndef WAREHOUSE_ALLOCATION_HPP
#define WAREHOUSE_ALLOCATION_HPP

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <stack>
#include <queue>
#include <set>
#include <algorithm>
#include <map>
#include <unordered_set>

struct ProblemData {
    int N, M;                               // N = Number of Warehouses, M = Number of Customers
    std::vector<int> CAP_W;                 // Warehouse - Capacity
    std::vector<double> S_W;                // Warehouse - Setup Cost
    std::vector<int> D_C;                   // Customer - Demand
    std::vector<std::vector<double>> T_CW;  // The deliver cost to warehouse for each customer

    ProblemData() = default;
    ProblemData (std::istream &input) {
        load(input);
    }

    void load(std::istream &input) {
        input >> N >> M;

        CAP_W.resize(N); 
        S_W.resize(N);
        D_C.resize(M);
        T_CW.resize(M, std::vector<double>(N));

        for (int i = 0; i < N; i++) {
            input >> CAP_W[i] >> S_W[i];
        }

        for (int i = 0; i < M; i++) {
            input >> D_C[i];
            for (int j = 0; j < N; j++) {
                input >> T_CW[i][j];
            }
        }
    }
};

struct ProblemSolution {
    ProblemData* P;
    std::vector<int> W_C;                                   // Solution - Warehouse of i-th customer
    std::vector<std::unordered_set<int>> C_W;               // Warehouse - Currently assigned customers
    std::vector<int> CAP_A_W;                               // Warehouse - Current capacity of i-th warehouse
    std::vector<std::vector<int>> CHEAPEST_W_C;             // Warehouses sorted by cost for each customer
    double fitness;                                         // Solution fitness

    ProblemSolution(ProblemData& P) {
        this->P = &P;
        W_C.resize(P.M, -1);
        C_W.resize(P.N, {});
        fitness = 0;
        
        precomputeCheapestWarehouses();
        fillWarehousesCapacity();
    }

    void precomputeCheapestWarehouses() {
        CHEAPEST_W_C.resize(P->M); // One sorted list per customer

        for (int i = 0; i < P->M; i++) {
            CHEAPEST_W_C[i].resize(P->N);
            std::iota(CHEAPEST_W_C[i].begin(), CHEAPEST_W_C[i].end(), 0);
            std::sort(CHEAPEST_W_C[i].begin(), CHEAPEST_W_C[i].end(),
                      [this, i](int w1, int w2) {
                          return P->T_CW[i][w1] < P->T_CW[i][w2];
                      });
        }
    }

    void fillWarehousesCapacity() {
        CAP_A_W.resize(P->N, 0);
        for (int w = 0; w != P->N; w++) {
            CAP_A_W[w] = P->CAP_W[w];
        }
    }

    // Warehouse
    bool WCanAccommodateC(int c, int w) {
        return CAP_A_W[w] >= P->D_C[c];
    }

    int randomWarehouse() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(0, P->N - 1);

        return dist(gen);
    }

    void deallocateWarehouse(int c) {
        const int w = W_C[c];
        if (w == -1)
            return;

        W_C[c] = -1;
        C_W[w].erase(c);
        CAP_A_W[w] += P->D_C[c];
        fitness -= P->T_CW[c][w];
        if (C_W[w].empty())
            fitness -= P->S_W[w];
    }

    void allocateWarehouse(int c, int w) {
        deallocateWarehouse(c);
        if (C_W[w].empty()) {
            fitness += P->S_W[w];
        }

        W_C[c] = w;
        C_W[w].insert(c);
        CAP_A_W[w] -= P->D_C[c];
        fitness += P->T_CW[c][w];
    }

    int findClosestAvailableWarehouse(int c, int exlude = -1) {
        int closestW = -1;
        for (const int& w : CHEAPEST_W_C[c]) {
            if (w != exlude && WCanAccommodateC(c, w)) {
                return w;
            }
        }
        return -1;
    }

    // Customer
    int randomCustomer() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(0, P->M - 1);

        return dist(gen);
    }

    double customerPenaltyContribution(int c) {
        if (c == -1) {
            return 0;
        } else {
            return P->T_CW[c][W_C[c]] + P->S_W[W_C[c]] / C_W[W_C[c]].size();
        }
    }

    int randomCustomerPenaltyBiased() {
        std::set<std::pair<double, int>> P_C;
        for (int c = 0; c != P->M; c++) {
            P_C.insert({customerPenaltyContribution(c), c});
        }
        std::vector<std::pair<double, int>> customers(P_C.begin(), P_C.end());
        std::vector<double> cumulative_weights;
        
        double total_weight = 0.0;
        for (size_t i = 0; i < customers.size(); ++i) {
            total_weight += (i*i + 1);
            cumulative_weights.push_back(total_weight);
        }
        
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<double> dist(0.0, total_weight);
        double random_value = dist(rng);
        
        auto it = std::lower_bound(cumulative_weights.begin(), cumulative_weights.end(), random_value);
        size_t index = std::distance(cumulative_weights.begin(), it);
        
        return customers[index].second;
    }

    // Layout modification
    void closestLayout() {
        for (int c = 0; c != P->M; c++) {
            int w = findClosestAvailableWarehouse(c);
            if (w != -1) {
                allocateWarehouse(c, w);
            } else {
                resolveHangingCustomer(c);
            }
        }
    }

    void kickRandomCustomer(bool heuristic = false) {
        int c = heuristic ? randomCustomerPenaltyBiased() : randomCustomer();
        int w = findClosestAvailableWarehouse(c, W_C[c]);
        if (w != -1) {
            allocateWarehouse(c, w);
        } else {
            resolveHangingCustomer(c);
        }
    }

    void resolveHangingCustomer(int c, bool heuristic = false) {
        std::stack<int> hangingCustomers;
        hangingCustomers.push(c);
        while (!hangingCustomers.empty()) {
            int hc = hangingCustomers.top(); 
            hangingCustomers.pop();

            // Close Down Random Warehouse
            int w = randomWarehouse();
            std::unordered_set<int> wh_customers = C_W[w];
            for (int c_ : wh_customers) {
                int w_ = findClosestAvailableWarehouse(c_, w);
                if (w_ != -1) {
                    allocateWarehouse(c_, w_);
                } else {
                    hangingCustomers.push(c_);
                }
            }

            // Assign the hanging customer to the warehouse
            if (!WCanAccommodateC(hc, w)) {
                throw std::runtime_error("Customer " + std::to_string(hc) + "cannot be assigned to warehouse " + 
                    std::to_string(w) + "that has been cleared out");
            }
            allocateWarehouse(hc, w);
        }
    }

    // Evaluation & Validation
    bool isValidSolution() {
        for (int i = 0; i != P->CAP_W.size(); i++) {
            if (CAP_A_W[i] > P->CAP_W[i]) {
                std::cout << "Invalid solution: Warehouse allocates more than capacity";
                return false;
            }
        }

        for (int& w : W_C) {
            if (w == -1) {
                std::cout << "Invalid solution: Customer not assigned to warehouse";
                return false;
            }
        }
        return true;
    }

    bool checkAllocation() {
        std::vector<int> allocates(P->N, 0);
        for (int c = 0; c != P->M; c++) {
            int& w = W_C[c];
            allocates[w] += P->D_C[c];
        }

        for (int i = 0; i != P->N; i++) {
            if (allocates[i] != CAP_A_W[i]) {
                return false;
            }
        }

        return true;
    }
};

struct ProblemStatistics {
    struct Epoch {
        int epoch;
        std::vector<ProblemSolution> solutions;
        std::chrono::duration<double> executionTime;

        double epochAverage() {
            double fitnessAvg = 0;
            for (ProblemSolution S : solutions) {
                fitnessAvg += S.fitness;
            }
            return fitnessAvg / solutions.size();
        }
    };

    struct Run {
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::vector<Epoch> epochs;

        Run() {
            start = std::chrono::high_resolution_clock::now();
        }
        
        int createRecord (int epoch, std::vector<ProblemSolution>& solutions) {
            Epoch R = Epoch();
            R.epoch = epoch;
            R.solutions = solutions;
            R.executionTime = std::chrono::high_resolution_clock::now() - start;
            epochs.push_back(R);

            return epochs.size();
        }

        int createRecord (int epoch, ProblemSolution solution) {
            Epoch R = Epoch();
            R.epoch = epoch;
            R.solutions = std::vector<ProblemSolution> {solution};
            R.executionTime = std::chrono::high_resolution_clock::now() - start;
            epochs.push_back(R);

            return epochs.size();
        }

        Epoch* lastEpoch() {
            return &(epochs.back());
        }

        int size() {
            return epochs.size();
        }
    };

    Run run;
    std::vector<Run> runs;

    int saveRun() {
        if (run.size()) {
            runs.push_back(run);
        }
        run = Run();
        return runs.size();
    }

    Run* getBestRun() {
        if (runs.empty()) {
            return nullptr;
        }

        Run* bestRun = &runs[0];
        double bestRunLastFitnessAvg = bestRun->lastEpoch()->epochAverage();
        for (Run& run : runs) {
            double runLastFitnessAvg = run.lastEpoch()->epochAverage();
            if (runLastFitnessAvg < bestRunLastFitnessAvg) {
                bestRun = &run;
                bestRunLastFitnessAvg = runLastFitnessAvg;
            }
        }
        return bestRun;
    }
};
#endif