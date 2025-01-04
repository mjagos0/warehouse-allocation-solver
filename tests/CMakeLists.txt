# tests/CMakeLists.txt

set(This WhAllocSolverTests)
set(TestData inputs/.debugger.in)

set(Sources
    test_fixture.hpp
    test_loader.cpp
    test_solution.cpp
    test_initialization.cpp
    test_warehouse.cpp
)

# Add test executable
add_executable(${This} ${Sources})

# Link GoogleTest and your main target
target_link_libraries(${This} PUBLIC
    PUBLIC
        gtest_main
        gtest
)

# Discover and add tests
add_test(NAME ${This} COMMAND ${This})
# add_test(NAME WL_16_1.MemoryAllocation COMMAND ${This} --gtest_filter=LoaderTests.LoadValidData)
# add_test(NAME WL_16_1.InputValues COMMAND ${This} --gtest_filter=LoaderTests.LoadInvalidData)
# add_test(NAME WL_16_1.InitializeSolutionTest COMMAND ${This} --gtest_filter=SolutionTests.SolveSimpleCase)
# add_test(NAME WL_16_1.ComplexSolutionTest COMMAND ${This} --gtest_filter=SolutionTests.SolveComplexCase)