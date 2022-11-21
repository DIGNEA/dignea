//
// Created by amarrero on 15/4/21.
//

#include <dignea/mea/evaluations/Weighted.h>
#include <dignea/mea/solutions/IKPSolution.h>

#include <utility>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Weighted tests", "[Weighted]") {
    SECTION("Creating a function") {
        float fitness = 0.8f;
        float novelty = 0.2f;
        auto f = Weighted<IKPSolution>(fitness, novelty);
        auto expected = std::tuple<float, float>(fitness, novelty);
        REQUIRE(f.getFAndNRatios() == expected);
    }
}