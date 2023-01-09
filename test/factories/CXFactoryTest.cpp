//
// Created by amarrero on 21/6/21.
//

#include <dignea/crossovers/OrderCrossover.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/factories/CXFactory.h>
#include <dignea/types/SolutionTypes.h>

#include <catch2/catch_all.hpp>

TEST_CASE("CXFactory Tests", "[CXFactory]") {
    SECTION("Creating an Uniform Crossover") {
        auto cx = CXFactory<FloatSolution>().create(CXType::Uniform);
        REQUIRE(cx);
        REQUIRE(cx->getName() == "Uniform One Crossover");
    }

    SECTION("Creating an Order Crossover") {
        auto cx = CXFactory<IntFloatSolution>().create(CXType::Order);
        REQUIRE(cx);
        REQUIRE(cx->getName() == "Order Crossover");
    }
}