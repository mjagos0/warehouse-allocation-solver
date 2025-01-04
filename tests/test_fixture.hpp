#ifndef TEST_FIXTURE_H
#define TEST_FIXTURE_H

#include <filesystem>
#include <fstream>
#include <string>

#include <gtest/gtest.h>
#include "../src/warehouse_allocation.hpp"

class WL_16_1 : public ::testing::Test {
protected:
    static inline const std::string InputDirectory = "../../inputs";
    static inline const std::string InputFile = "wl_16_1.in";
    static inline ProblemData P;

    static void SetUpTestSuite() {
        if (!std::filesystem::exists(InputDirectory) || 
            !std::filesystem::is_directory(InputDirectory)) {
            FAIL() << "Input directory is missing. Current path: " << std::filesystem::current_path();
        }

        std::ifstream file(InputDirectory + '/' + InputFile);
        if (!file.is_open()) {
            FAIL() << "Testing file inputs/wl_16_1.in is missing.";
        }

        P.load(file);
        file.close();
    }
};

#endif // TEST_FIXTURE_H
