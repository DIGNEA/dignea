#include <dignea/algorithms/kp_heuristics/MPW.h>

#include <string>

#include <catch2/catch_all.hpp>

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the MPW KP Heuristic"
    "[MPW]") {
    SECTION("MPW can be instantiated") {
        auto mpw = MPW();
        REQUIRE(mpw.getName() == "MPW KP");
        REQUIRE(mpw.getID() == "MPW");
    }

    SECTION("Get json info") {
        auto mpw{MPW()};
        REQUIRE(mpw.to_json()["name"] == "MPW KP");
    }

    SECTION("MPW can solve a KP instance") {
        auto mpw{MPW()};
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
        auto mpw{MPW()};
        mpw.setProblem(knp);
        REQUIRE_NOTHROW(mpw.run());
        auto solution = mpw.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherMPW{MPW()};
            otherMPW.setProblem(knp);
            otherMPW.run();
            auto nSolution = otherMPW.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }
}