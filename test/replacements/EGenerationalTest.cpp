//
// Created by amarrero on 2/7/21.
//

#include "catch2/catch.hpp"
#include <dignea/replacements/EGenerational.h>
#include <dignea/types/SolutionTypes.h>
#include <vector>

using namespace std;

TEST_CASE("EGenerational Replacement Tests", "[EGenerational]") {
    const int nSolutions = 100;
    const int dimension = 100;
    auto replace = make_unique<EGenerational<FloatSolution>>();
    vector<FloatSolution> solutions;
    solutions.reserve(nSolutions);
    for (int i = 0; i < nSolutions; i++) {
        solutions.emplace_back(dimension, 1);
        solutions[i].setFitness(i);
    }

    SECTION("Testing EGenerational getName") {
        REQUIRE(replace->getName() == "EGenerational");
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

    SECTION("Replace Elitist Generational works perfectly") {
        vector<FloatSolution> offspring;
        offspring.reserve(nSolutions);
        for (int i = 0; i < nSolutions; i++) {
            offspring.emplace_back(dimension, 1);
            offspring[i].setFitness(0);
        }

        vector population = replace->replace(solutions, offspring);
        REQUIRE(population[0].getFitness() == 99);
        for (int i = 1; i < nSolutions; i++) {
            REQUIRE(0 == population[i].getFitness());
        }
    }
}