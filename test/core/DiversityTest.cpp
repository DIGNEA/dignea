#include <dignea/core/Diversity.h>
#include <dignea/distances/Hamming.h>
#include <dignea/types/SolutionTypes.h>

#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Diversity tests", "[Diversity]") {
    SECTION("Test average Hamming distance in population") {
        IntDoubleSolution individual1(10, 1);
        IntDoubleSolution individual2(10, 1);
        IntDoubleSolution individual3(10, 1);
        vector vars1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        vector vars2 = {1, 2, 3, 4, 5, 60, 7, 8, 9, 10};
        vector vars3 = {1, 2, 2, 4, 5, 6, 4, 8, 9, 0};
        individual1.setVariables(vars1);
        individual2.setVariables(vars2);
        individual3.setVariables(vars3);
        vector<IntDoubleSolution> population = {individual1, individual2,
                                                individual3};
        int expected = 8 / 3;
        auto hamming = make_unique<Hamming<int>>();
        int result = Diversity<int32_t>(move(hamming)).compute(population);
        REQUIRE(expected == result);
    }

    SECTION("Test average Hamming distance in population is 0") {
        const int popSize = 100;
        vector<IntDoubleSolution> population;
        population.reserve(popSize);
        for (int i = 0; i < popSize; i++) {
            IntDoubleSolution solution(popSize, 1);
            vector<int> variables(popSize);
            std::iota(begin(variables), end(variables), 0);
            solution.setVariables(variables);
            population.push_back(solution);
        }
        int expected = 0;
        auto hamming = make_unique<Hamming<int>>();
        int result = Diversity<int>(move(hamming)).compute(population);
        REQUIRE(expected == result);
    }
}