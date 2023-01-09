

#include <dignea/evolution/Evolution.h>
#include <dignea/types/SolutionTypes.h>

#include <map>
#include <vector>

#include <catch2/catch_all.hpp>
using namespace std;

TEST_CASE("Evolution Test", "[Evolution]") {
  SECTION("Creating a empty evolution") {
    auto evolution = Evolution<FloatSolution>();
    REQUIRE_NOTHROW(evolution.results());
    REQUIRE(evolution.results().empty());
  }

  SECTION("Including solutions in evolution") {
    auto evolution = Evolution<FloatSolution>();
    const int nSolutions = 100;
    for (int i = 0; i < nSolutions; i++) {
      FloatSolution solution(100, 1, 0);
      solution.setFitness(float(i));
      vector tmpVector = {solution};
      evolution.update(i, tmpVector);
    }
    REQUIRE_NOTHROW(evolution.results());
  }

  SECTION("Computing evolution solutions in evolution") {
    auto evolution = Evolution<FloatSolution>();
    const int nSolutions = 100;
    map<int, float> evolutionResults;
    for (int i = 0; i < nSolutions; i++) {
      FloatSolution solution(100, 1, 0);
      solution.setFitness(float(i));
      vector tmpVector = {solution};
      evolution.update(i, tmpVector);
      evolutionResults.insert({i, i});
    }
    REQUIRE_NOTHROW(evolution.results());
    REQUIRE(evolutionResults == evolution.results());
  }
}