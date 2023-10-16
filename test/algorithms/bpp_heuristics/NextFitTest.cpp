/**
 * @file NextFitTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-01
 *
 * Test cases for the NextFit heuristic
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/algorithms/bpp_heuristics/NextFit.h>

#include <catch2/catch_all.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the NextFit Heuristic"
    "[NextFit]") {
    SECTION("NextFit can be instantiated") {
        auto nextFit = NextFit();
        REQUIRE(nextFit.getName() == "NextFit");
        REQUIRE(nextFit.getID() == "NextFit");
    }

    SECTION("Get json info") {
        auto nextFit{NextFit()};
        REQUIRE(nextFit.to_json()["name"] == "NextFit");
    }

    SECTION("NextFit can solve a BPP instance") {
        auto nextFit{NextFit()};
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);
        REQUIRE_FALSE(bpp == nullptr);
        nextFit.setProblem(bpp);
        REQUIRE_NOTHROW(nextFit.run());
        REQUIRE_FALSE(nextFit.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);

        auto repetitions = 9;
        auto nextFit{NextFit()};
        nextFit.setProblem(bpp);
        REQUIRE_NOTHROW(nextFit.run());

        auto solution = nextFit.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherNextFit{NextFit()};
            otherNextFit.setProblem(bpp);
            otherNextFit.run();
            auto nSolution = otherNextFit.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }

    SECTION("Check the solution of the NextFit heuristic") {
        vector items = {2, 5, 4, 7, 1, 3, 8};
        auto binCapacity = 10;

        auto bpp = make_shared<BPP>(items.size(), binCapacity, items);
        auto nextFit{NextFit()};
        nextFit.setProblem(bpp);
        REQUIRE_NOTHROW(nextFit.run());
        auto front = nextFit.getResults();
        vector solutions = front.getSolutions();
        auto nfFitness = solutions[0].getFitness();
        // Evaluating the solution with the BPP evaluate code
        auto solCloned(solutions[0]);
        bpp->evaluate(solCloned);
        auto bppFitness = solCloned.getFitness();
        REQUIRE(areEqual(nfFitness, 0.404f));
        REQUIRE(bppFitness == nfFitness);
    }
}