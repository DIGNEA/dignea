//
// Created by amarrero on 15/4/21.
//

#include <dignea/generator/evaluations/Weighted.h>
#include <dignea/generator/instances/KPInstance.h>

#include <utility>

#include <catch2/catch_all.hpp>

using namespace std;

TEST_CASE("Weighted tests", "[Weighted]") {
    SECTION("Creating a function") {
        float fitness = 0.8f;
        float novelty = 0.2f;
        auto f = Weighted<KPInstance>(fitness, novelty);
        auto expected = std::tuple<float, float>(fitness, novelty);
        REQUIRE(f.getFAndNRatios() == expected);
    }
}