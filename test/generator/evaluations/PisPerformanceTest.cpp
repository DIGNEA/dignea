/**
 * @file PisPerformanceTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/generator/evaluations/PisPerformance.h>

#include <algorithm>
#include <catch2/catch_all.hpp>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("Pisinger Performance evaluator tests", "[PisPerformance]") {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1.0, 15.0);
    vector<float> runtimes(100);
    std::generate(runtimes.begin(), runtimes.end(), [&]() { return dis(gen); });
    runtimes[0] = 0.05f;  // Best runtime

    SECTION("Compute fitness of PisPerormance data") {
        auto minRuntime =
            *std::min_element(runtimes.begin() + 1, runtimes.end());
        auto difference = minRuntime - runtimes[0];
        PisPerformance evaluator;
        float result = evaluator.compute(runtimes);
        REQUIRE(result == difference);
    }
}