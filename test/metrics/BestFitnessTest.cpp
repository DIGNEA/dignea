//
// Created by amarrero on 5/4/21.
//

#include <dignea/core/Front.h>
#include <dignea/metrics/BestFitness.h>
#include <dignea/problems/Sphere.h>

#include <vector>

#include <catch2/catch_all.hpp>

using namespace std;

TEST_CASE("BestFitness Metric Test", "[BestFitness]") {
    SECTION("Creating a new instance of BestFitness") {
        auto bestFitness = make_unique<BestFitness<FloatSolution>>();
        REQUIRE(bestFitness != nullptr);
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
        }
        // Setting the best
        solutions[0].setFitness(fitness);
        Front<FloatSolution> front(solutions);
        BestFitness<FloatSolution> bestFitness;
        REQUIRE(bestFitness.computeValue(front) == fitness);
        REQUIRE(bestFitness.getBestSolution(front) == solutions[0]);
    }

    SECTION("Computing the value with float solutions") {
        int numSolutions = 10;
        float winner = 10000;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0, 100);
        vector<float> solutions;
        for (int i = 0; i < numSolutions - 1; i++) {
            solutions.push_back(dist(gen));
        }
        solutions.push_back(winner);
        BestFitness<FloatSolution> bestFitness;
        REQUIRE(bestFitness.computeValue(solutions) == winner);
    }

    SECTION("Computing the value with empty float solutions") {
        vector<float> solutions{};
        BestFitness<FloatSolution> bestFitness;
        REQUIRE(bestFitness.computeValue(solutions) == 0.0f);
    }

    SECTION("Computing the value with empty Front") {
        Front<FloatSolution> front(0);
        BestFitness<FloatSolution> bestFitness;
        // Las soluciones no tienen fitness aún
        REQUIRE(bestFitness.computeValue(front) == 0);
    }

    SECTION("Get the best solution with empty Front") {
        Front<FloatSolution> front(0);
        BestFitness<FloatSolution> bestFitness;
        // Las soluciones no tienen fitness aún
        REQUIRE(bestFitness.getBestSolution(front).getFitness() == 0);
    }
}