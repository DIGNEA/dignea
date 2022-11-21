
#include <dignea/distances/Hamming.h>
#include <dignea/types/SolutionTypes.h>

#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Hamming distance tests", "[Hamming]") {
  SECTION("Test distance between vectors") {
    vector<int> ind1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> ind2 = {1, 2, 3, 4, 5, 60, 7, 8, 9, 10};
    int expected = 1;
    auto hamming = Hamming<int>();
    int result = hamming.compute(ind1, ind2);
    REQUIRE(expected == result);

    ind1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ind2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    expected = 0;
    result = hamming.compute(ind1, ind2);
    REQUIRE(expected == result);

    ind1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ind2 = {1, 2, 2, 4, 5, 6, 4, 8, 9, 0};
    expected = 3;
    result = hamming.compute(ind1, ind2);
    REQUIRE(expected == result);
  }
}