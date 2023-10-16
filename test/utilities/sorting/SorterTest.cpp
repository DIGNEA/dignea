//
// Created by amarrero on 15/12/20.
//

#include <dignea/core/Problem.h>
#include <dignea/problems/SampleProblem.h>
#include <dignea/problems/ZDT.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <catch2/catch_all.hpp>
#include <cmath>
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

    SECTION("Fast Non-Dominated Sorting test") {
        vector<FloatSolution> population{};
        auto problem = make_unique<ZDT>(ZDT_MODE::ZDT1, 10);
        for (int i = 0; i < numOfSolutions; i++) {
            FloatSolution solution(10, 2);
            solution.setObjAt(0, 0);
            solution.setObjAt(1, 0.5);
            population.push_back(move(solution));
        }
        auto fronts = fastNonDominatedSort(population, problem.get());
        REQUIRE(!fronts.empty());
        REQUIRE(fronts.size() == 1);
        REQUIRE(fronts[0].size() == (unsigned)numOfSolutions);
        for (auto solution : fronts[0]) {
            REQUIRE(solution.getRank() == 0);
        }
    }

    SECTION("Fast Non-Dominated Sorting test - One front per solution") {
        vector<FloatSolution> population{};
        auto problem = make_unique<ZDT>(ZDT_MODE::ZDT1, 10);
        // We create 5 solutions with predefined objectives to get the expected
        // results
        auto nSols = 5;
        auto incr = 0.01f;
        auto currentIncr = 0.0f;
        for (int i = 0; i < nSols; i++) {
            FloatSolution solution(10, 2);
            solution.setObjAt(0, 0);
            solution.setObjAt(1, 0.5 + currentIncr);
            population.push_back(move(solution));
            currentIncr += incr;
        }
        auto fronts = fastNonDominatedSort(population, problem.get());
        REQUIRE(!fronts.empty());
        REQUIRE(fronts.size() == (unsigned)nSols);
        int rank = 0;
        currentIncr = 0;
        const auto relative_difference_factor = 0.0001;  // 0.01%
        for (auto front : fronts) {
            float obj1 = front[0].getObjAt(1);
            float obj2 = 0.5 + currentIncr;
            REQUIRE(front.size() == (unsigned)1);
            REQUIRE(front[0].getRank() == rank);
            REQUIRE(front[0].getObjAt(0) == 0.0f);
            const auto greater_magnitude =
                std::max(std::abs(obj1), std::abs(obj2));
            REQUIRE(std::abs(obj1 - obj2) <
                    relative_difference_factor * greater_magnitude);
            rank++;
            currentIncr += incr;
        }
    }

    SECTION("Fast Non-Dominated Sorting test - 3 fronts") {
        vector<FloatSolution> population{};
        auto problem = make_unique<ZDT>(ZDT_MODE::ZDT1, 10);
        // We create 5 solutions with predefined objectives to get the expected
        // results
        auto nSols = 5;
        vector<pair<float, float>> objectives = {
            {0.5, 0.0}, {0.0, 0.5}, {0.6, 0.6}, {1.0, 0.6}, {1.0, 0.6}};
        for (int i = 0; i < nSols; i++) {
            FloatSolution solution(10, 2);
            solution.setObjAt(0, objectives[i].first);
            solution.setObjAt(1, objectives[i].second);
            population.push_back(move(solution));
        }

        auto fronts = fastNonDominatedSort(population, problem.get());
        auto expectedFronts = 3;
        // We expect the following outcome
        // F0 = [(0.5, 0.0), (0.0, 0.5)]
        // F1 = [(0.6, 0.6)]
        // F2 = [(1.0, 0.6), (1.0, 0.6)]
        REQUIRE(!fronts.empty());
        REQUIRE(fronts.size() == (unsigned)expectedFronts);
        REQUIRE(fronts[0].size() == (unsigned)2);
        REQUIRE(fronts[1].size() == (unsigned)1);
        REQUIRE(fronts[2].size() == (unsigned)2);
    }
}