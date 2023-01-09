#include <dignea/algorithms/kp_heuristics/MaP.h>

#include <string>

#include <catch2/catch_all.hpp>

using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the MaP KP Heuristic"
    "[MaP]") {
    SECTION("MaP can be instantiated") {
        auto mpw = MaP();
        REQUIRE(mpw.getName() == "MaP KP");
        REQUIRE(mpw.getID() == "MaP");
    }

    SECTION("Get json info") {
        auto mpw{MaP()};
        REQUIRE(mpw.to_json()["name"] == "MaP KP");
    }

    SECTION("MaP can solve a KP instance") {
        auto mpw{MaP()};
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
        auto mpw{MaP()};
        mpw.setProblem(knp);
        REQUIRE_NOTHROW(mpw.run());
        auto solution = mpw.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherMaP{MaP()};
            otherMaP.setProblem(knp);
            otherMaP.run();
            auto nSolution = otherMaP.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }
}