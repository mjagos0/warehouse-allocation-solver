#include <filesystem>
#include <fstream>
#include <string>

#include <gtest/gtest.h>
#include "../src/warehouse_allocation.hpp"
#include "test_fixture.hpp"

TEST_F(WL_16_1, MemoryAllocation) {
    ASSERT_EQ(P.N, 16);
    ASSERT_EQ(P.M, 50);

    ASSERT_EQ(P.CAP_W.size(), 16);
    ASSERT_EQ(P.S_W.size(), 16);
    ASSERT_EQ(P.D_C.size(), 50);
    ASSERT_EQ(P.T_CW.size(), 50);
    ASSERT_EQ(P.T_CW[0].size(), 16);
}

TEST_F(WL_16_1, InputValues) {
    // WH capacity
    EXPECT_EQ(P.CAP_W[0], 5000);
    EXPECT_EQ(P.CAP_W[15], 5000);

    // Customer capacity demanded
    EXPECT_EQ(P.D_C[0], 146);
    EXPECT_EQ(P.D_C[49], 222);

    // WH setup cost
    EXPECT_NEAR(P.S_W[0], 7500, 1e-3);
    EXPECT_NEAR(P.S_W[10], 0, 1e-3);
    EXPECT_NEAR(P.S_W[15], 7500, 1e-3);

    // Customer distance from given WH
    EXPECT_NEAR(P.T_CW[0][0], 6739.725, 1e-3);
    EXPECT_NEAR(P.T_CW[0][15], 6051.7, 1e-3);
    EXPECT_NEAR(P.T_CW[49][0], 7095.675, 1e-3);
    EXPECT_NEAR(P.T_CW[49][15], 7448.1, 1e-3);
}