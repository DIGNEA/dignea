//
// Created by amarrero on 15/12/20.
//

#include <dignea/utilities/random/ParallelPRNG.h>

#include <catch2/catch_all.hpp>

TEST_CASE("ParallelPRNG tests", "[ParallelPRNG]") {
    ParallelPRNG prng;

    SECTION("RandomNormal values in range") {
        double mean = 0.0, std = 1.0;
        double generated = prng.randNormal(mean, std);
        REQUIRE(generated >= -3.0);
        REQUIRE(generated <= 3.0);
    }

    SECTION("Random value in Sphere") {
        auto generated = prng.randSphere(30);
        REQUIRE(generated != nullptr);
        delete (generated);
    }
}