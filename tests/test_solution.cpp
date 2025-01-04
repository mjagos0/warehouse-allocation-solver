// #include <gtest/gtest.h>
// #include "../src/warehouse_allocation.hpp"
// #include "test_fixture.hpp"

// TEST(SolutionTests, SolutionIsValidTest) {
//     ProblemData P;
//     P.M = 5;
//     P.N = 3;
//     P.CAP_W = {5, 10, 15};
//     P.S_W = {1, 2, 3};
//     P.D_C = {1, 2, 3, 4, 5};
//     P.T_CW = {{1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}, {5, 10, 15}};

//     ProblemSolution S(P);
//     S.allocateWarehouse(0, 0);
//     S.allocateWarehouse(1, 0);
//     S.allocateWarehouse(2, 1);
//     S.allocateWarehouse(3, 1);
//     S.allocateWarehouse(4, 2);
//     ASSERT_TRUE(S.isValidSolution());

//     S.allocateWarehouse(0, 1);
//     S.allocateWarehouse(1, 1);
//     ASSERT_TRUE(S.isValidSolution());
// }

// TEST_F(WL_16_1, AllocateWarehouseTest) {
//     ProblemSolution S(P);
//     S.initializeSolution();
//     S.allocateWarehouse(0, 1);

//     // Allocate WH 1 to Cust 0
//     ASSERT_EQ(S.SolutionC[0], 1);
//     ASSERT_EQ(S.GoodsAllocatedW[1], P.D_C[0]);

//     // Allocate WH 2 to Cust 0
//     S.allocateWarehouse(0, 2);
//     ASSERT_EQ(S.SolutionC[0], 2);
//     ASSERT_EQ(S.GoodsAllocatedW[1], 0);
//     ASSERT_EQ(S.GoodsAllocatedW[2], P.D_C[0]);

//     // Allocate WH 2 to Cust 1
//     S.allocateWarehouse(1, 2);
//     ASSERT_EQ(S.SolutionC[1], 2);
//     ASSERT_EQ(S.GoodsAllocatedW[2], P.D_C[0] + P.D_C[1]);

//     // Allocate WH 3 to Cust 0
//     S.allocateWarehouse(0, 3);
//     ASSERT_EQ(S.SolutionC[0], 3);
//     ASSERT_EQ(S.GoodsAllocatedW[2], P.D_C[1]);
//     ASSERT_EQ(S.GoodsAllocatedW[3], P.D_C[0]);

//     // Deallocate all warehouses
//     S.deallocateWarehouse(0);
//     S.deallocateWarehouse(1);
//     ASSERT_TRUE(std::all_of(S.SolutionC.begin(), S.SolutionC.end(), [](int c) { return c == -1; }));
//     ASSERT_TRUE(std::all_of(S.GoodsAllocatedW.begin(), S.GoodsAllocatedW.end(), [](int alloc) { return alloc == 0; }));

//     // Attempt to deallocate warehouse of cust without warehouse
//     S.deallocateWarehouse(0);
//     ASSERT_TRUE(std::all_of(S.SolutionC.begin(), S.SolutionC.end(), [](int c) { return c == -1; }));
//     ASSERT_TRUE(std::all_of(S.GoodsAllocatedW.begin(), S.GoodsAllocatedW.end(), [](int alloc) { return alloc == 0; }));
// }

// TEST_F(WL_16_1, FindAvailableWarehousesTest) {
//     ProblemSolution S(P);
//     EXPECT_EQ(S.findAvailableWarehouses(10).size(), 16);

//     S.allocateWarehouse(17, 0);
//     EXPECT_EQ(S.findAvailableWarehouses(10).size(), 15);
//     EXPECT_EQ(S.findAvailableWarehouses(0).size(), 16);

//     S.deallocateWarehouse(17);
//     EXPECT_EQ(S.findAvailableWarehouses(10).size(), 16);

//     S.allocateWarehouse(0, 0);
//     S.allocateWarehouse(16, 1);
//     S.allocateWarehouse(2, 2);
//     S.allocateWarehouse(3, 3);
//     S.allocateWarehouse(17, 4);
//     EXPECT_EQ(S.findAvailableWarehouses(33).size(), 11);
//     EXPECT_EQ(S.findAvailableWarehouses(0, S.SolutionC[0]).size(), 15);

//     S.allocateWarehouse(5, 5);
//     S.allocateWarehouse(6, 6);
//     S.allocateWarehouse(7, 7);
//     S.allocateWarehouse(18, 8);
//     S.allocateWarehouse(19, 9);
//     S.allocateWarehouse(10, 10);
//     S.allocateWarehouse(11, 11);
//     S.allocateWarehouse(12, 12);
//     S.allocateWarehouse(13, 13);
//     S.allocateWarehouse(14, 14);
//     S.allocateWarehouse(15, 15);
//     EXPECT_EQ(S.findAvailableWarehouses(33).size(), 0);
// }

// TEST_F(WL_16_1, FindRandomAvailableWarehousesTest) {
//     ProblemSolution S(P);
//     std::vector<int> wAvailable;
//     int w;

//     for (int i = 0; i != 1000; i++) {
//         wAvailable = S.findAvailableWarehouses(33);
//         w = S.findRandomAvailableWarehouse(33);
//         EXPECT_TRUE(std::find(wAvailable.begin(), wAvailable.end(), w) != wAvailable.end()) << w;
//     }

//     S.allocateWarehouse(0, 0);
//     S.allocateWarehouse(16, 1);
//     S.allocateWarehouse(2, 2);
//     S.allocateWarehouse(3, 3);
//     S.allocateWarehouse(17, 4);
//     S.allocateWarehouse(5, 5);
//     S.allocateWarehouse(6, 6);
//     S.allocateWarehouse(7, 7);
//     S.allocateWarehouse(18, 8);
//     S.allocateWarehouse(19, 9);
//     S.allocateWarehouse(10, 10);

//     for (int i = 0; i != 1000; i++) {
//         wAvailable = S.findAvailableWarehouses(33);
//         w = S.findRandomAvailableWarehouse(33);
//         EXPECT_TRUE(std::find(wAvailable.begin(), wAvailable.end(), w) != wAvailable.end()) << w;
//     }

//     S.allocateWarehouse(11, 11);
//     S.allocateWarehouse(12, 12);
//     S.allocateWarehouse(13, 13);
//     S.allocateWarehouse(14, 14);
//     S.allocateWarehouse(15, 15);

//     EXPECT_EQ(S.findRandomAvailableWarehouse(33), -1);
// }

// TEST_F(WL_16_1, RandomSolutionTest) {
//     int RANDOM_SOLUTIONS = 1000;

//     // Check if Random solutions generate valid solutions
//     ProblemSolution S(P);
//     for (int i = 0; i != RANDOM_SOLUTIONS; i++) {
//         S.randomSolution();
//         ASSERT_TRUE(S.checkAllocation());
//         ASSERT_TRUE(S.isValidSolution());
//     }
// }

// TEST_F(WL_16_1, ResolveHangingCustomerTest) {
//     ProblemSolution S(P);
//     for (int i = 0; i != 1000; i++) {
//         S.randomSolution();
//         int w = S.SolutionC[33];
//         S.resolveHangingCustomer(33);
//         ASSERT_NE(w, S.SolutionC[33]);
//         ASSERT_TRUE(S.isValidSolution());
//         ASSERT_TRUE(S.checkAllocation());
//     }
// }

// TEST_F(WL_16_1, KickCustomerTest) {
//     ProblemSolution S(P);
//     for (int i = 0; i != 1000; i++) {
//         S.randomSolution();
//         int c = S.randomCustomer();
//         int w = S.SolutionC[c];
//         S.assignRandomWarehouse(c);
    
//         ASSERT_NE(w, S.SolutionC[c]);
//         ASSERT_TRUE(S.isValidSolution());
//         ASSERT_TRUE(S.checkAllocation());
//     }
// }

// TEST_F(WL_16_1, FitnessFunctionTest) {
//     ProblemSolution S(P);
//     for (int i = 0; i != 1000; i++) {
//         S.randomSolution();
//         S.evaluateFitness();
//         double fitness = S.fitness;

//         S.kickRandomCustomer();
//         S.evaluateFitness();

//         ASSERT_NE(fitness, S.fitness);
//     }
// }

// TEST_F(WL_16_1, PenaltyContributionTest) {
//     ProblemSolution S(P);
//     S.randomSolution();
//     S.evaluateFitness();

//     double sum;
//     for (int c = 0; c != P.M; c++) {
//         sum += S.customerPenaltyContribution(c);
//     }
//     ASSERT_TRUE(sum - S.fitness < 0.1f) << "Offset: " << sum - S.fitness;

//     S.kickRandomCustomer();
//     S.kickRandomCustomer();
//     S.kickRandomCustomer();
//     S.kickRandomCustomer();
//     S.kickRandomCustomer();

//     sum = 0;
//     for (int c = 0; c != P.M; c++) {
//         sum += S.customerPenaltyContribution(c);
//     }
//     S.evaluateFitness();
//     ASSERT_TRUE(sum - S.fitness < 0.1f) << "Offset: " << sum - S.fitness;
// }

// // TEST_F(WL_16_1, randomCustomerPenaltyBiasedTest) {
// //     ProblemSolution S(P);
    
// // }