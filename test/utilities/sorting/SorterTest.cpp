//
// Created by amarrero on 15/12/20.
//

#include <dignea/core/Problem.h>
#include <dignea/problems/SampleProblem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <catch2/catch.hpp>
#include <vector>

TEST_CASE("Sorter can be created", "[Sorter]") {
    int numOfSolutions = 10;
    int dimension = 30;
    int numObjectives = 2;
    int lowerObj = 0;
    int upperObj = 100;
    std::vector<FloatSolution> solutions;

    SECTION("Sorting by first objective - MINIMIZE") {
        solutions.reserve(numOfSolutions);
        auto sphere = make_shared<SampleProblem>(dimension);
        for (int i = 0; i < numOfSolutions; i++) {
            FloatSolution solution(dimension, numObjectives);
            double obj = PseudoRandom::randDouble(lowerObj, upperObj);
            solution.setObjAt(0, obj);
            solutions.push_back(solution);
        }
        // Ordenamos a minimizar
        sortByObj(solutions, 0, sphere.get());
        for (unsigned int i = 0; i < solutions.size() - 1; i++) {
            REQUIRE(solutions[i].getObjAt(0) <= solutions[i + 1].getObjAt(0));
        }
    }
    SECTION("Sorting by first objective - MAXIMIZE") {
        solutions.reserve(numOfSolutions);
        auto sample = make_shared<SampleProblem>(dimension);
        for (int i = 0; i < numOfSolutions; i++) {
            FloatSolution solution(dimension, numObjectives);
            double obj = PseudoRandom::randDouble(lowerObj, upperObj);
            solution.setObjAt(1, obj);
            solutions.push_back(solution);
        }
        // Ordenamos a minimizar
        sortByObj(solutions, 1, sample.get());
        for (unsigned int i = 0; i < solutions.size() - 1; i++) {
            REQUIRE(solutions[i].getObjAt(0) >= solutions[i + 1].getObjAt(0));
        }
    }

    SECTION("Sorting by fitness") {
        solutions.reserve(numOfSolutions);
        for (int i = 0; i < numOfSolutions; i++) {
            FloatSolution solution(dimension, numObjectives);
            float fitness = PseudoRandom::randDouble(lowerObj, upperObj);
            solution.setFitness(fitness);
            solutions.push_back(solution);
        }
        sortByFitness(solutions);
        for (unsigned int i = 0; i < solutions.size() - 1; i++) {
            REQUIRE(solutions[i].getFitness() >= solutions[i + 1].getFitness());
        }
    }

    SECTION("Sorting throws population empty") {
        auto sphere = make_shared<SampleProblem>(dimension);
        vector<FloatSolution> solutions;
        REQUIRE_THROWS(sortByObj(solutions, 0, sphere.get()));
        REQUIRE_THROWS(sortByFitness(solutions));
    }

    SECTION("Sorting throws out-of-range") {
        solutions.reserve(numOfSolutions);
        auto sample = make_shared<SampleProblem>(dimension);
        for (int i = 0; i < numOfSolutions; i++) {
            FloatSolution solution(dimension, numObjectives);
            double obj = PseudoRandom::randDouble(lowerObj, upperObj);
            solution.setObjAt(1, obj);
            solutions.push_back(move(solution));
        }
        // Ordenamos a minimizar
        REQUIRE_THROWS(sortByObj(solutions, numObjectives + 2, sample.get()));
    }
}