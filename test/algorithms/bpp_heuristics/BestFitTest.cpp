/**
 * @file BestFitTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <dignea/algorithms/bpp_heuristics/BestFit.h>
#include <dignea/utilities/Comparator.h>

#include <catch2/catch_all.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the BestFit Heuristic"
    "[BestFit]") {
    SECTION("BestFit can be instantiated") {
        auto bestFit = BestFit();
        REQUIRE(bestFit.getName() == "BestFit");
        REQUIRE(bestFit.getID() == "BestFit");
    }

    SECTION("Get json info") {
        auto bestFit{BestFit()};
        REQUIRE(bestFit.to_json()["name"] == "BestFit");
    }

    SECTION("BestFit can solve a BPP instance") {
        auto bestFit{BestFit()};
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);
        REQUIRE_FALSE(bpp == nullptr);
        bestFit.setProblem(bpp);
        REQUIRE_NOTHROW(bestFit.run());
        REQUIRE_FALSE(bestFit.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_shared<BPP>(path);

        auto repetitions = 9;
        auto bestFit{BestFit()};
        bestFit.setProblem(bpp);
        REQUIRE_NOTHROW(bestFit.run());

        auto solution = bestFit.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherBestFit{BestFit()};
            otherBestFit.setProblem(bpp);
            otherBestFit.run();
            auto nSolution = otherBestFit.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }

    SECTION("Check the solution of the BestFit heuristic") {
        vector items = {2, 5, 4, 7, 1, 3, 8};
        auto binCapacity = 10;

        auto bpp = make_shared<BPP>(items.size(), binCapacity, items);
        auto bestFit{BestFit()};
        bestFit.setProblem(bpp);
        REQUIRE_NOTHROW(bestFit.run());
        auto front = bestFit.getResults();
        vector solutions = front.getSolutions();
        auto wfFitness = solutions[0].getFitness();
        // Evaluating the solution with the BPP evaluate code
        auto solCloned(solutions[0]);
        bpp->evaluate(solCloned);
        auto bppFitness = solCloned.getFitness();
        REQUIRE(bppFitness == wfFitness);
    }
}