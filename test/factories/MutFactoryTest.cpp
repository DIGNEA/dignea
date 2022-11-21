//
// Created by amarrero on 21/6/21.
//

#include <dignea/factories/MutFactory.h>
#include <dignea/types/SolutionTypes.h>

#include "catch2/catch.hpp"

TEST_CASE("MutFactory Tests", "[MutFactory]") {
  SECTION("Creating an Uniform One Mutation") {
    auto mut = MutFactory<FloatSolution>().create(MutType::UniformOne);
    REQUIRE(mut);
    REQUIRE(mut->getName() == "Uniform One Mutation");
  }

  SECTION("Creating an Uniform All Mutation") {
    auto mut = MutFactory<FloatSolution>().create(MutType::UniformAll);
    REQUIRE(mut);
    REQUIRE(mut->getName() == "Uniform All Mutation");
  }

  SECTION("Creating an Swap Mutation") {
    auto mut = MutFactory<IntFloatSolution>().create(MutType::SwapMutation);
    REQUIRE(mut);
    REQUIRE(mut->getName() == "Swap Mutation");
  }
}