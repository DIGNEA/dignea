//
// Created by amarrero on 5/4/21.
//

#include <dignea/core/Front.h>
#include <dignea/metrics/WorstFitness.h>
#include <dignea/problems/Sphere.h>

#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("WorstFitness Metric Test", "[WorstFitness]") {
    SECTION("Creating a new instance of WorstFitness") {
        auto worstFitness = make_unique<WorstFitness<FloatSolution>>();
        REQUIRE(worstFitness != nullptr);
    }

    SECTION("Computing the value with Sphere solutions") {
        int numSolutions = 10;
        float fitness = 100.0f;
        int dimension = 30;
        unique_ptr<Sphere> sphere = make_unique<Sphere>(dimension);
        vector<FloatSolution> solutions;
        solutions.reserve(numSolutions);
        for (int i = 0; i < numSolutions; i++) {
            solutions.push_back(sphere->createSolution());
            solutions[i].setFitness(fitness);
        }
        // Setting the best
        solutions[0].setFitness(-fitness);
        Front<FloatSolution> front(solutions);
        WorstFitness<FloatSolution> worstFitness;
        REQUIRE(worstFitness.computeValue(front) == -fitness);
        REQUIRE(worstFitness.getWorstSolution(front) == solutions[0]);
    }

    SECTION("Computing the value with empty Front") {
        Front<FloatSolution> front(0);
        WorstFitness<FloatSolution> worstFitness;
        REQUIRE(worstFitness.computeValue(front) == 0);
    }

    SECTION("Get the best solution with empty Front") {
        Front<FloatSolution> front(0);
        WorstFitness<FloatSolution> worstFitness;
        CHECK_THROWS(worstFitness.getWorstSolution(front));
    }

    SECTION("Computing the value with float solutions") {
        int numSolutions = 10;
        float winner = 0;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(10, 100);
        vector<float> solutions;
        for (int i = 0; i < numSolutions - 1; i++) {
            solutions.push_back(dist(gen));
        }
        solutions.push_back(winner);
        WorstFitness<FloatSolution> worstF;
        REQUIRE(worstF.computeValue(solutions) == winner);
    }

    SECTION("Computing the value with empty float solutions") {
        vector<float> solutions{};
        WorstFitness<FloatSolution> worstF;
        REQUIRE(worstF.computeValue(solutions) == 0.0f);
    }
}
