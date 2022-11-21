
#include <dignea/distances/Euclidean.h>
#include <dignea/types/SolutionTypes.h>

#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Euclidean distance tests", "[Euclidean]") {
  SECTION("Test distance between vectors") {
    vector<int> ind1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    vector<int> ind2 = {1, 2, 3, 4, 5, 60, 7, 8, 9, 10, 11};
    int expected = 54;
    int result = Euclidean<int>().compute(ind1, ind2);
    REQUIRE(expected == result);

    ind1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    ind2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    expected = 0;
    result = Euclidean<int>().compute(ind1, ind2);
    REQUIRE(expected == result);

    ind1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    ind2 = {1, 2, 2, 4, 5, 6, 4, 8, 9, 0, 11};
    expected = 10;
    result = Euclidean<int>().compute(ind1, ind2);
    REQUIRE(expected == result);
  }
}