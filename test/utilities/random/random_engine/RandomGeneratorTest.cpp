//
// Created by amarrero on 15/12/20.
//

#include <catch2/catch_all.hpp>
#include <dignea/utilities/random/RandomGenerator.h>

TEST_CASE("RandomGenerator can be created", "[RandomGenerator]") {
    RandomGenerator randomGenerator;

    SECTION("Integers in range") {
        int lower = 0, upper = 10;
        int generated = randomGenerator.rnd(lower, upper);
        REQUIRE(generated >= lower);
        REQUIRE(generated <= upper);
    }

    SECTION("Double in range") {
        double lower = 0.0, upper = 5.0;
        double generated = randomGenerator.rndreal(lower, upper);
        REQUIRE(generated >= lower);
        REQUIRE(generated <= upper);
    }
}