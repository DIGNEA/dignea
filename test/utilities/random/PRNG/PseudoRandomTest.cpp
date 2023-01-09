//
// Created by amarrero on 15/12/20.
//

#include <dignea/utilities/random/PseudoRandom.h>

#include <catch2/catch_all.hpp>

TEST_CASE("PseudoRandom can be created", "[PseudoRandom]") {
    PseudoRandom pseudoRandom;

    SECTION("PseudoRandom random is not null") {
        REQUIRE(pseudoRandom.randomGenerator_ != nullptr);
    }

    SECTION("PseudoRandom random set to nullptr") {
        PseudoRandom::randomGenerator_ = nullptr;
        REQUIRE(pseudoRandom.randomGenerator_ == nullptr);
    }

    SECTION("Double between [0, 1]") {
        double generated = PseudoRandom::randDouble();
        REQUIRE(generated >= 0.0);
        REQUIRE(generated <= 1.0);
    }

    SECTION("Double between [0, 1] with nullptr random") {
        PseudoRandom::randomGenerator_ = nullptr;
        REQUIRE(pseudoRandom.randomGenerator_ == nullptr);
        double generated = PseudoRandom::randDouble();
        REQUIRE(generated >= 0.0);
        REQUIRE(generated <= 1.0);
    }

    SECTION("Double in range") {
        int lower = 0, upper = 1;
        int repetitions = 100;
        for (int i = 0; i < repetitions; i++) {
            double generated = PseudoRandom::randDouble(lower, upper);
            REQUIRE(generated >= lower);
            REQUIRE(generated <= upper);
            upper++;
        }
    }

    SECTION("Double in range with nullptr") {
        PseudoRandom::randomGenerator_ = nullptr;
        REQUIRE(pseudoRandom.randomGenerator_ == nullptr);
        int lower = 0, upper = 1;
        int repetitions = 100;
        for (int i = 0; i < repetitions; i++) {
            double generated = PseudoRandom::randDouble(lower, upper);
            REQUIRE(generated >= lower);
            REQUIRE(generated <= upper);
            upper++;
        }
    }

    SECTION("Integer in range") {
        int lower = 1, upper = 100;
        int repetitions = 10000;
        for (int i = 0; i < repetitions; i++) {
            int generated = PseudoRandom::randInt(lower, upper);
            REQUIRE(generated >= lower);
            REQUIRE(generated <= upper);
        }
    }

    SECTION("Integer in increasing range") {
        int lower = 0, upper = 2;
        int repetitions = 1000;
        for (int i = 0; i < repetitions; i++) {
            int generated = PseudoRandom::randInt(lower, upper);
            REQUIRE(generated >= lower);
            REQUIRE(generated <= upper);
            upper++;
        }
    }

    SECTION("Integer indexes no duplicated") {
        int idx2Swap = 0;
        int idx2Swap2 = 0;
        int vars = 51;
        while (idx2Swap == idx2Swap2) {
            idx2Swap = PseudoRandom::randInt(0, vars);
            idx2Swap2 = PseudoRandom::randInt(0, vars);
        }
        REQUIRE(idx2Swap <= vars);
        REQUIRE(idx2Swap >= 0);
        REQUIRE(idx2Swap2 <= vars);
        REQUIRE(idx2Swap2 >= 0);
        REQUIRE(idx2Swap != idx2Swap2);
    }

    SECTION("Integer in range with nullptr") {
        PseudoRandom::randomGenerator_ = nullptr;
        REQUIRE(pseudoRandom.randomGenerator_ == nullptr);
        int lower = 0, upper = 1;
        int repetitions = 100;
        for (int i = 0; i < repetitions; i++) {
            int generated = PseudoRandom::randInt(lower, upper);
            REQUIRE(generated >= lower);
            REQUIRE(generated <= upper);
            upper++;
        }
    }

    SECTION("Gaussian Distribution") {
        double mean = 0.0, deviation = 1.0;
        double generated = PseudoRandom::randNormal(mean, deviation);
        REQUIRE(generated != 0.0);
    }

    SECTION("Random Sphere") {
        int dimension = 2;
        int repetition = 20;
        for (int i = 0; i < repetition; i++) {
            REQUIRE_FALSE(*PseudoRandom::randSphere(dimension) == 0);
            dimension++;
        }
    }
}