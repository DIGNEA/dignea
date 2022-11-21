//
// Created by amarrero on 2/7/21.
//

#include "catch2/catch.hpp"
#include <dignea/replacements/FirstImprove.h>
#include <dignea/types/SolutionTypes.h>
#include <vector>

using namespace std;

TEST_CASE("FirstImprove Replacement Tests", "[FirstImprove]") {
    const int nSolutions = 100;
    const int dimension = 100;
    auto replace = make_unique<FirstImprove<FloatSolution>>();
    vector<FloatSolution> solutions;
    solutions.reserve(nSolutions);
    for (int i = 0; i < nSolutions; i++) {
        solutions.emplace_back(dimension, 1);
        solutions[i].setFitness(i);
    }

    SECTION("Testing FirstImprove getName") {
        REQUIRE(replace->getName() == "First Improve");
    }

    SECTION("Replace with position not allowed") {
        REQUIRE_THROWS(replace->replace(solutions, solutions[0], 0));
    }

    SECTION("Replace throws if sizes are different") {
        vector<FloatSolution> offspring;
        offspring.push_back(solutions[0]);
        REQUIRE_THROWS(replace->replace(solutions, offspring));
    }

    SECTION("Replace throws if offspring empty") {
        vector<FloatSolution> offspring;
        REQUIRE_THROWS(replace->replace(solutions, offspring));
    }

    SECTION("Replace throws if population empty") {
        vector<FloatSolution> offspring;
        vector<FloatSolution> pop;
        offspring.push_back(solutions[0]);
        REQUIRE_THROWS(replace->replace(pop, offspring));
    }

    SECTION("Replace throws if both empty") {
        vector<FloatSolution> offspring;
        vector<FloatSolution> pop;
        REQUIRE_THROWS(replace->replace(pop, offspring));
    }

    SECTION("Replace First Improve works perfectly") {
        vector<FloatSolution> offspring;
        offspring.reserve(nSolutions);
        for (int i = 0; i < nSolutions; i++) {
            offspring.emplace_back(dimension, 1);
            // Solo los pares serán mejores
            if (i % 2 == 0)
                offspring[i].setFitness(i * 10);
        }

        vector population = replace->replace(solutions, offspring);
        for (int i = 0; i < nSolutions; i++) {
            float expected = i;
            if (i % 2 == 0) {
                expected *= 10;
            }
            REQUIRE(expected == population[i].getFitness());
        }
    }

    SECTION("Replace First Improve works perfectly with constraints") {
        vector<FloatSolution> solutions;
        vector<FloatSolution> offspring;
        solutions.reserve(nSolutions);
        offspring.reserve(nSolutions);

        for (int i = 0; i < nSolutions; i++) {
            solutions.emplace_back(dimension, 1, 1);
            solutions[i].setFitness(i);
            offspring.emplace_back(dimension, 1, 1);
            // Solo los pares serán mejores
            if (i % 2 == 0) {
                solutions[i].setConstAt(0, 100);
                offspring[i].setConstAt(0, 0);
            }
        }

        vector population = replace->replace(solutions, offspring);
        for (int i = 0; i < nSolutions; i++) {
            REQUIRE(0 == population[i].getConstAt(0));
        }
    }
}