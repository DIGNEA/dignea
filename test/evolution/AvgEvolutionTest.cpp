

#include <dignea/evolution/AvgEvolution.h>
#include <dignea/types/SolutionTypes.h>

#include <map>
#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("AvgEvolution Test", "[AvgEvolution]") {
  SECTION("Creating a empty evolution") {
    auto evolution = AvgEvolution<FloatSolution>();
    REQUIRE_NOTHROW(evolution.results());
    REQUIRE(evolution.results().empty());
  }

  SECTION("Including solutions in evolution") {
    auto evolution = AvgEvolution<FloatSolution>();
    const int nSolutions = 100;
    for (int i = 0; i < nSolutions; i++) {
      vector<FloatSolution> tmpVector;
      tmpVector.reserve(2);
      for (int j = 0; j < 2; j++) {
        FloatSolution solution(100, 1, 0);
        solution.setFitness(float(i + j));
        solution.setConstraintCoeff(0);
        tmpVector.push_back(solution);
      }
      evolution.update(i, tmpVector);
    }
    REQUIRE_NOTHROW(evolution.results());
    REQUIRE(evolution.results().size() == nSolutions);
  }

  SECTION("Computing evolution solutions in evolution") {
    auto evolution = AvgEvolution<FloatSolution>();
    const int nSolutions = 100;
    map<int, float> evolutionResults;
    for (int i = 0; i < nSolutions; i++) {
      vector<FloatSolution> tmpVector;
      tmpVector.reserve(2);
      float avgFitness = 0;
      for (int j = 0; j < 2; j++) {
        FloatSolution solution(100, 1, 0);
        solution.setFitness(float(i + j));
        solution.setConstraintCoeff(0);
        tmpVector.push_back(solution);
        avgFitness += float(i + j);
      }
      evolution.update(i, tmpVector);
      avgFitness /= 2;
      evolutionResults.insert({i, avgFitness});
    }
    REQUIRE_NOTHROW(evolution.results());
    REQUIRE(evolution.results().size() == nSolutions);
    REQUIRE(evolution.results() == evolutionResults);
  }
}