/**
 * @file FirstFitTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <dignea/algorithms/bpp_heuristics/FirstFit.h>

#include <catch2/catch_all.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the FirstFit Heuristic"
    "[FirstFit]") {
    SECTION("FirstFit can be instantiated") {
        auto firstFit = FirstFit();
        REQUIRE(firstFit.getName() == "FirstFit");
        REQUIRE(firstFit.getID() == "FirstFit");
    }

    SECTION("Get json info") {
        auto firstFit{FirstFit()};
        REQUIRE(firstFit.to_json()["name"] == "FirstFit");
    }

    SECTION("FirstFit can solve a BPP instance") {
        auto firstFit{FirstFit()};
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);
        REQUIRE_FALSE(bpp == nullptr);
        firstFit.setProblem(bpp);
        REQUIRE_NOTHROW(firstFit.run());
        REQUIRE_FALSE(firstFit.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);

        auto repetitions = 9;
        auto firstFit{FirstFit()};
        firstFit.setProblem(bpp);
        REQUIRE_NOTHROW(firstFit.run());

        auto solution = firstFit.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherFirstFit{FirstFit()};
            otherFirstFit.setProblem(bpp);
            otherFirstFit.run();
            auto nSolution = otherFirstFit.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }

    SECTION("Check the solution of the FirstFit heuristic") {
        vector items = {2, 5, 4, 7, 1, 3, 8};
        auto binCapacity = 10;

        auto bpp = make_shared<BPP>(items.size(), binCapacity, items);
        auto firstFit{FirstFit()};
        firstFit.setProblem(bpp);
        REQUIRE_NOTHROW(firstFit.run());
        auto front = firstFit.getResults();
        vector solutions = front.getSolutions();
        auto nfFitness = solutions[0].getFitness();
        // Evaluating the solution with the BPP evaluate code
        auto solCloned(solutions[0]);
        bpp->evaluate(solCloned);
        auto bppFitness = solCloned.getFitness();
        REQUIRE(nfFitness == 0.565f);
        REQUIRE(bppFitness == nfFitness);
    }
}