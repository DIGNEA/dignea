//
// Created by amarrero on 22/6/21.
//

#include <catch2/catch_all.hpp>
#include <dignea/factories/SelFactory.h>
#include <dignea/selections/BinaryTournamentSelection.h>
#include <dignea/types/SolutionTypes.h>

TEST_CASE("SelFactory Tests", "[SelFactory]") {
    SECTION("Creating a Binary Tournament Selection") {
        auto sel = SelFactory<FloatSolution>().create(
                SelType::Binary);
        REQUIRE(sel);
        REQUIRE(sel->getName() == "Binary Tournament Selection");
    }
}