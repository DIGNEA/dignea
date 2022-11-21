//
// Created by amarrero on 2/7/21.
//

#include "catch2/catch.hpp"
#include <dignea/replacements/ReplaceWorst.h>
#include <dignea/types/SolutionTypes.h>
#include <vector>

using namespace std;

TEST_CASE("ReplaceWorst Replacement Tests", "[ReplaceWorst]") {
    const int nSolutions = 100;
    const int dimension = 100;
    auto replace = make_unique<ReplaceWorst<FloatSolution>>();
    vector<FloatSolution> solutions;
    solutions.reserve(nSolutions);
    for (int i = 0; i < nSolutions; i++) {
        solutions.emplace_back(dimension, 1);
        solutions[i].setFitness(i);
    }

    SECTION("Testing ReplaceWorst getName") {
        REQUIRE(replace->getName() == "Replace Worst");
    }

    SECTION("Replace with position not allowed") {
        REQUIRE_THROWS(replace->replace(solutions, solutions[0], 0));
    }

    SECTION("Replace throws if offspring empty") {
        vector<FloatSolution> offspring;
        REQUIRE_THROWS(replace->replace(solutions, offspring));
    }

    SECTION("Replace throws if len(offspring) != 1") {
        vector<FloatSolution> offspring;
        offspring.push_back(solutions[0]);
        offspring.push_back(solutions[1]);
        REQUIRE_THROWS(replace->replace(solutions, offspring));
    }

    SECTION("Replace throws if population empty") {
        vector<FloatSolution> offspring;
        vector<FloatSolution> pop;
        offspring.push_back(solutions[0]);
        REQUIRE_THROWS(replace->replace(pop, offspring));
    }

    SECTION("Replace Worst works perfectly") {
        vector<FloatSolution> offspring;
        offspring.emplace_back(dimension, 1);
        offspring[0].setFitness(100);
        vector population = replace->replace(solutions, offspring);
        for (int i = 0; i < nSolutions - 2; i++) {
            REQUIRE(population[i].getFitness() >
                    population[i + 1].getFitness());
        }
        REQUIRE(population[nSolutions - 1].getFitness() == 100);
    }
}