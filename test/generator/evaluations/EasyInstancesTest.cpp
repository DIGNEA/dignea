//
// Created by amarrero on 15/4/21.
//

#include <dignea/generator/evaluations/EasyInstances.h>

#include <vector>

#include <catch2/catch_all.hpp>

using namespace std;

TEST_CASE("EasyInstances tests", "[EasyInstances]") {
    SECTION("Compute fitness of EasyInstances") {
        int numSolutions = 100;
        float maxFitness = 1e5;
        vector<float> solutions;
        solutions.reserve(numSolutions);
        solutions.push_back(maxFitness);  // Setting the target
        for (int i = 1; i < numSolutions; i++) {
            solutions.push_back(float(i));
        }
        float diff = maxFitness - solutions[numSolutions - 1];
        EasyInstances easyInstances;
        float result = easyInstances.compute(solutions);
        REQUIRE(result == diff);
    }
}