#include <gtest/gtest.h>
#include "../src/warehouse_allocation.hpp"
#include "test_fixture.hpp"

TEST_F(WL_16_1, InitializeSolutionTest) {
    ProblemSolution S(P);

    ASSERT_EQ(S.W_C.size(), P.M);
    ASSERT_EQ(S.C_W.size(), P.N);
    ASSERT_EQ(S.CAP_A_W.size(), P.N);
    ASSERT_EQ(S.CHEAPEST_W_C.size(), P.M);

    ASSERT_TRUE(std::all_of(S.W_C.begin(), S.W_C.end(), [](int c) { return c == -1; })) << "Customer warehouses are not set";
    ASSERT_EQ(S.fitness, 0.0) << "Fitness is initialized to zero.";

    // Warehouses are max capacity
    for (int w = 0; w != P.N; w++) {
        ASSERT_EQ(S.CAP_A_W[w], P.CAP_W[w]);
    }
}

TEST_F(WL_16_1, PrecomputeCheapestWarehousesTest) {
    ProblemSolution S(P);
    for (int c = 0; c != P.M; c++) {
        for (int i = 0; i != P.N - 1; i++) {
            double costThis = P.T_CW[c][S.CHEAPEST_W_C[c][i]];
            double costNext = P.T_CW[c][S.CHEAPEST_W_C[c][i+1]];
            ASSERT_TRUE(costNext >= costThis);
        }
    }
}