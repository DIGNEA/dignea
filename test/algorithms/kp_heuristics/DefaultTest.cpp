#include <dignea/algorithms/kp_heuristics/Default.h>

#include <string>

#include "catch2/catch.hpp"

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the Def KP Heuristic"
    "[Def]") {
    SECTION("Default can be instantiated") {
        auto def = Default();
        REQUIRE(def.getName() == "Default KP");
        REQUIRE(def.getID() == "Def");
    }

    SECTION("Get json info") {
        auto def{Default()};
        REQUIRE(def.to_json()["name"] == "Default KP");
    }

    SECTION("Default can solve a KP instance") {
        auto def{Default()};
        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        auto knp = make_shared<KPNR>(path);
        REQUIRE_FALSE(knp == nullptr);
        def.setProblem(knp);
        REQUIRE_NOTHROW(def.run());
        REQUIRE_FALSE(def.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        auto knp = make_shared<KPNR>(path);
        auto repetitions = 9;
        auto def{Default()};
        def.setProblem(knp);
        REQUIRE_NOTHROW(def.run());
        auto solution = def.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherDEF{Default()};
            otherDEF.setProblem(knp);
            otherDEF.run();
            auto nSolution = otherDEF.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }
}