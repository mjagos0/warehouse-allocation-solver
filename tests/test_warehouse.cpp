#include <gtest/gtest.h>
#include "../src/warehouse_allocation.hpp"
#include "test_fixture.hpp"

TEST_F(WL_16_1, WarehouseTests) {
    ProblemSolution S(P);
    ASSERT_TRUE(S.WCanAccommodateC(0, 0));
    for (int i = 0; i != 100; i++) {
        int w = S.randomWarehouse();
        S.allocateWarehouse(0, w);
        ASSERT_DOUBLE_EQ(S.fitness, P.T_CW[0][w] + P.S_W[w]);
    }

    S.deallocateWarehouse(0);
    ASSERT_DOUBLE_EQ(S.fitness, 0);
}