/**
 * @file WorstFitTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <dignea/algorithms/bpp_heuristics/WorstFit.h>
#include <dignea/utilities/Comparator.h>

#include <catch2/catch_all.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the WorstFit Heuristic"
    "[WorstFit]") {
    SECTION("WorstFit can be instantiated") {
        auto worstFit = WorstFit();
        REQUIRE(worstFit.getName() == "WorstFit");
        REQUIRE(worstFit.getID() == "WorstFit");
    }

    SECTION("Get json info") {
        auto worstFit{WorstFit()};
        REQUIRE(worstFit.to_json()["name"] == "WorstFit");
    }

    SECTION("WorstFit can solve a BPP instance") {
        auto worstFit{WorstFit()};
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);
        REQUIRE_FALSE(bpp == nullptr);
        worstFit.setProblem(bpp);
        REQUIRE_NOTHROW(worstFit.run());
        REQUIRE_FALSE(worstFit.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);

        auto repetitions = 9;
        auto worstFit{WorstFit()};
        worstFit.setProblem(bpp);
        REQUIRE_NOTHROW(worstFit.run());

        auto solution = worstFit.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherWorstFit{WorstFit()};
            otherWorstFit.setProblem(bpp);
            otherWorstFit.run();
            auto nSolution = otherWorstFit.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }

    SECTION("Check the solution of the WorstFit heuristic") {
        vector items = {2, 5, 4, 7, 1, 3, 8};
        auto binCapacity = 10;

        auto bpp = make_shared<BPP>(items.size(), binCapacity, items);
        auto worstFit{WorstFit()};
        worstFit.setProblem(bpp);
        REQUIRE_NOTHROW(worstFit.run());
        auto front = worstFit.getResults();
        vector solutions = front.getSolutions();
        auto wfFitness = solutions[0].getFitness();
        // Evaluating the solution with the BPP evaluate code
        auto solCloned(solutions[0]);
        bpp->evaluate(solCloned);
        auto bppFitness = solCloned.getFitness();
        REQUIRE(bppFitness == wfFitness);
    }
}