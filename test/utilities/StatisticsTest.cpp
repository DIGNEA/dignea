//
// Created by amarrero on 23/6/21.
//

#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/Comparator.h>
#include <dignea/utilities/Statistics.h>

#include <catch2/catch_all.hpp>
#include <cmath>

TEST_CASE("Statistics tests", "[Statistics]") {
    const int dimension = 100;
    const int nObjs = 1;
    SECTION("Compute the EIGN of individual") {
        FloatSolution solution(dimension, nObjs);
        int begin = 0;
        vector<float> vars(dimension, 0.0);
        std::generate(vars.begin(), vars.end(), [&]() { return ++begin; });
        solution.setVariables(vars);
        float expected = (dimension + 1.0) / 2;
        REQUIRE(expected == mean(solution.getVariables()));
    }

    SECTION("Compute the Variance of individual") {
        FloatSolution solution(dimension, nObjs);
        int begin = 0;
        vector<float> vars(dimension, 0.0);
        std::generate(vars.begin(), vars.end(), [&]() { return ++begin; });
        solution.setVariables(vars);
        double expected = (dimension * dimension - 1.0) / 12.0;
        double mean = (dimension + 1.0) / 2.0;
        REQUIRE(expected == variance(mean, solution.getVariables()));
    }

    SECTION("Compute the Standard Deviation of individual") {
        FloatSolution solution(dimension, nObjs);
        int begin = 0;
        vector<float> vars(dimension, 0.0);
        std::generate(vars.begin(), vars.end(), [&]() { return ++begin; });
        solution.setVariables(vars);
        double expected = sqrt((dimension * dimension - 1.0) / 12.0);
        double mean = (dimension + 1.0) / 2.0;
        REQUIRE(expected == standardDev(mean, solution.getVariables()));
    }

    SECTION("Compute the EIGN of vector of floats") {
        int begin = 0;
        vector<float> vars(dimension, 0.0);
        std::generate(vars.begin(), vars.end(), [&]() { return ++begin; });
        float expected = (dimension + 1.0) / 2;
        REQUIRE(expected == mean(vars));
    }

    SECTION("Compute the Variance of vector of floats") {
        int begin = 0;
        vector<float> vars(dimension, 0.0);
        std::generate(vars.begin(), vars.end(), [&]() { return ++begin; });
        double expected = (dimension * dimension - 1.0) / 12.0;
        double mean = (dimension + 1.0) / 2.0;
        REQUIRE(expected == variance(mean, vars));
    }

    SECTION("Compute the Standard Deviation of vector of floats") {
        int begin = 0;
        vector<float> vars(dimension, 0.0);
        std::generate(vars.begin(), vars.end(), [&]() { return ++begin; });
        double expected = sqrt((dimension * dimension - 1.0) / 12.0);
        double mean = (dimension + 1.0) / 2.0;
        REQUIRE(expected == standardDev(mean, vars));
    }
    //----
    SECTION("Compute the EIGN of matrix of floats") {
        vector<vector<float>> matrix{{1, 2, 3}, {4, 5, 6}, {6, 6, 6}};
        double expected = 39.0 / 9.0;
        REQUIRE(expected == mean(matrix));
    }

    SECTION("Compute the Variance of matrix of floats") {
        vector<vector<float>> matrix{{1, 2, 3}, {4, 5, 6}, {6, 6, 6}};
        auto mean = 39.0 / 9.0;
        double expected = 3.3333333333;
        REQUIRE(expected <= variance(mean, matrix));
    }

    SECTION("Compute the Standard Deviation of vector of floats") {
        vector<vector<float>> matrix{{1, 2, 3}, {4, 5, 6}, {6, 6, 6}};
        auto mean = 39.0 / 9.0;
        double expected = 1.825;
        REQUIRE(expected <= standardDev(mean, matrix));
    }

    SECTION("Normalizes a vector with 4 items") {
        vector data = {3.0f, 4.0f, 1.0f, 2.0f};
        vector normalized = normalize(data);
        vector expected = {0.5477225575f, 0.7302967433f, 0.1825741858f,
                           0.3651483717f};
        REQUIRE(areEqual(normalized, expected));
    }

    SECTION("Normalizes a sequential vector with 4 items") {
        vector data = {1.0f, 2.0f, 3.0f, 4.0f};
        vector normalized = normalize(data);
        vector expected = {0.1825741858f, 0.3651483717f, 0.5477225575f,
                           0.7302967433f};
        REQUIRE(areEqual(normalized, expected));
    }

    SECTION("Normalizes a vector with 4 zeros") {
        vector data = {0.0f, 0.0f, 0.0f, 0.0f};
        vector normalized = normalize(data);
        vector expected = {0.0f, 0.0f, 0.0f, 0.0f};
        REQUIRE(areEqual(normalized, expected));
    }

    SECTION("Normalizes a vector with 3 negative numbers") {
        vector data = {-2.0f, 4.0f, -6.0f};
        vector normalized = normalize(data);
        vector expected = {-0.2672612419f, 0.5345224838f, -0.8017837257f};
        REQUIRE(areEqual(normalized, expected));
    }

    SECTION("Median function returns NaN if the vector is empty") {
        vector<float> data = {};
        auto result = median(data);
        REQUIRE(std::numeric_limits<int>::quiet_NaN() == result);
    }

    SECTION("Median works on vector [1, 3, 5, 7] -> 4") {
        vector data = {1, 3, 5, 7};
        auto expected = 4;
        auto result = median(data);
        REQUIRE(expected == result);
    }

    SECTION(
        "Median works with negative numbers ([-5, -5, -3, -4, 0, -1]) -> "
        "-3.0") {
        vector<float> data = {-5.0, -5.0, -3.0, -4.0, 0.0, -1.0};
        auto expected = -3.0;
        auto result = median(data);
        REQUIRE(expected == result);
    }
}