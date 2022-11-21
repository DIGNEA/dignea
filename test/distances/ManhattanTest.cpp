//
// Created by amarrero on 5/7/21.
//

#include "catch2/catch.hpp"
#include <dignea/distances/Manhattan.h>
#include <vector>

TEST_CASE("Manhattan Distance tests" "[Manhattan Distance]") {

    SECTION("Computing the distance between two vectors") {
        auto ind1 = std::vector<float>{1.0, 2.0, 3.0, 3.0, 5.0};
        auto ind2 = std::vector<float>{1.0, 2.0, 3.0, 4.0, 5.0};
        float expected = 1;
        auto euclidean = Manhattan<float>();
        REQUIRE(expected == euclidean.compute(ind1, ind2));
    }

    SECTION("Computing the distance throws exception") {
        auto ind1 = std::vector<float>{1.0, 2.0, 3.0, 3.0, 5.0, 6.0};
        auto ind2 = std::vector<float>{1.0, 2.0, 3.0, 4.0, 5.0};
        auto euclidean = Manhattan<float>();
        REQUIRE_THROWS(euclidean.compute(ind1, ind2));
    }
}