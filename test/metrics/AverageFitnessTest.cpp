//
// Created by amarrero on 5/4/21.
//

#include <dignea/core/Front.h>
#include <dignea/metrics/AverageFitness.h>
#include <dignea/problems/Sphere.h>

#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("AverageFitness Metric Test", "[AverageFitness]") {
    SECTION("Creating a new instance of AverageFitness") {
        auto averageFitness = make_unique<AverageFitness<FloatSolution>>();
        REQUIRE(averageFitness != nullptr);
    }

    SECTION("Computing the value with Sphere solutions") {
        int numSolutions = 10;
        float fitness = 0.0f;
        int dimension = 30;
        unique_ptr<Sphere> sphere = make_unique<Sphere>(dimension);
        vector<FloatSolution> solutions;
        solutions.reserve(numSolutions);
        for (int i = 0; i < numSolutions; i++) {
            solutions.push_back(sphere->createSolution());
        }
        Front<FloatSolution> front(solutions);
        AverageFitness<FloatSolution> averageFitness;
        // Las soluciones no tienen fitness aún
        REQUIRE(averageFitness.computeValue(front) == fitness);
    }

    SECTION("Computing the value with Sphere solutions biased") {
        int numSolutions = 10;
        float fitness = 10.0;
        int dimension = 30;
        unique_ptr<Sphere> sphere = make_unique<Sphere>(dimension);
        vector<FloatSolution> solutions;
        solutions.reserve(numSolutions);
        for (int i = 0; i < numSolutions; i++) {
            solutions.push_back(sphere->createSolution());
            solutions[i].setFitness(fitness);
        }
        Front<FloatSolution> front(solutions);
        AverageFitness<FloatSolution> averageFitness;
        REQUIRE(averageFitness.computeValue(front) == fitness);
    }

    SECTION("Computing the value with float solutions") {
        int numSolutions = 10;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0, 100);
        vector<float> solutions;
        float expected = 0;
        for (int i = 0; i < numSolutions; i++) {
            auto s = dist(gen);
            expected += s;
            solutions.push_back(s);
        }
        expected /= numSolutions;
        AverageFitness<FloatSolution> avgFitness;
        REQUIRE(abs((avgFitness.computeValue(solutions) - expected)) < 1e-3);
    }

    SECTION("Computing the value with empty float solutions") {
        vector<float> solutions{};
        AverageFitness<FloatSolution> avgFitness;
        REQUIRE(avgFitness.computeValue(solutions) == 0.0f);
    }
}