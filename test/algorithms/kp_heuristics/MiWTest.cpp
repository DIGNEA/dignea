#include <dignea/algorithms/kp_heuristics/MiW.h>

#include <string>

#include "catch2/catch.hpp"

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the MiW KP Heuristic"
    "[MiW]") {
    SECTION("MiW can be instantiated") {
        auto mpw = MiW();
        REQUIRE(mpw.getName() == "MiW KP");
        REQUIRE(mpw.getID() == "MiW");
    }

    SECTION("Get json info") {
        auto mpw{MiW()};
        REQUIRE(mpw.to_json()["name"] == "MiW KP");
    }

    SECTION("MiW can solve a KP instance") {
        auto mpw{MiW()};
        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        auto knp = make_shared<KPNR>(path);
        REQUIRE_FALSE(knp == nullptr);
        mpw.setProblem(knp);
        REQUIRE_NOTHROW(mpw.run());
        REQUIRE_FALSE(mpw.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        auto knp = make_shared<KPNR>(path);
        auto repetitions = 9;
        auto mpw{MiW()};
        mpw.setProblem(knp);
        REQUIRE_NOTHROW(mpw.run());
        auto solution = mpw.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherMiW{MiW()};
            otherMiW.setProblem(knp);
            otherMiW.run();
            auto nSolution = otherMiW.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }
}