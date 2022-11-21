//
// Created by amarrero on 23/6/21.
//

#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/Statistics.h>

#include <cmath>

#include "catch2/catch.hpp"

TEST_CASE("Statistics tests", "[Statistics]") {
    const int dimension = 100;
    const int nObjs = 1;
    SECTION("Compute the MEAN of individual") {
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

    SECTION("Compute the MEAN of vector of floats") {
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
    SECTION("Compute the MEAN of matrix of floats") {
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
}