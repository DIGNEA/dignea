//
// Created by amarrero on 5/7/21.
//

#include "catch2/catch.hpp"
#include <dignea/distances/Hamming.h>
#include <vector>

TEST_CASE("Hamming Distance tests" "[Hamming Distance]") {

    SECTION("Computing the distance between two vectors") {
        auto ind1 = std::vector<float>{1.0, 2.0, 3.0, 3.0, 5.0};
        auto ind2 = std::vector<float>{1.0, 2.0, 3.0, 4.0, 5.0};
        float expected = 1;
        auto hamming = Hamming<float>();
        REQUIRE(expected == hamming.compute(ind1, ind2));
    }

    SECTION("Computing the distance throws exception") {
        auto ind1 = std::vector<float>{1.0, 2.0, 3.0, 3.0, 5.0, 6.0};
        auto ind2 = std::vector<float>{1.0, 2.0, 3.0, 4.0, 5.0};
        auto hamming = Hamming<float>();
        REQUIRE_THROWS(hamming.compute(ind1, ind2));
    }
}