cmake_minimum_required(VERSION 3.10)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Project Name and Language
set(This WhAllocSolver)
project(${This} VERSION 1.0 LANGUAGES CXX)

# Headers & Sources
set(Headers
    src/warehouse_allocation.hpp
    src/local_search.hpp
    src/evolutionary_algorithm.hpp
    src/memetic_algorithm.hpp
)
set(Sources
    src/main.cpp
)

# Create the executable
add_executable(${This} ${Sources})

# --- GoogleTest Integration ---
enable_testing()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Add tests subdirectory
add_subdirectory(tests)

# --- FetchContent for nlohmann/json ---
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)
target_link_libraries(${This} PRIVATE nlohmann_json::nlohmann_json)