#include <dignea/algorithms/tsp_heuristics/TwoOpt.h>

#include <string>

#include <catch2/catch_all.hpp>
using namespace tsp_heuristics;
using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the TwoOpt TSP Heuristic"
    "[TwoOpt TSP]") {
    SECTION("TwoOpt can be instantiated") {
        auto nn = TwoOpt();
        REQUIRE(nn.getName() == "TwoOpt TSP");
        REQUIRE(nn.getID() == "TwoOpt TSP");
    }

    SECTION("TwoOpt can be instantiated with NN initialisation") {
        auto nn = TwoOpt(false);
        REQUIRE(nn.getName() == "NN-TwoOpt TSP");
        REQUIRE(nn.getID() == "NN-TwoOpt TSP");
    }

    SECTION("Get json info") {
        auto nn{TwoOpt()};
        REQUIRE(nn.to_json()["name"] == "TwoOpt TSP");
    }

    SECTION("TwoOpt can solve a TSP instance") {
        auto nn{TwoOpt()};
        std::string path = "../data/instances/TSP/example1.tsp";
        auto tsp = make_shared<TSP>(path);
        REQUIRE_FALSE(tsp == nullptr);
        nn.setProblem(tsp);
        REQUIRE_NOTHROW(nn.run());
        REQUIRE_FALSE(nn.getResults().empty());
    }

    SECTION("TwoOpt can solve a TSP instance when NN initilisation is used") {
        auto nn{TwoOpt(false)};
        std::string path = "../data/instances/TSP/example1.tsp";
        auto tsp = make_shared<TSP>(path);
        REQUIRE_FALSE(tsp == nullptr);
        nn.setProblem(tsp);
        REQUIRE_NOTHROW(nn.run());
        REQUIRE_FALSE(nn.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path = "../data/instances/TSP/example1.tsp";
        auto tsp = make_shared<TSP>(path);
        auto repetitions = 5;
        auto nn{TwoOpt()};
        nn.setProblem(tsp);
        REQUIRE_NOTHROW(nn.run());
        auto solution = nn.getResults();
        auto obj = solution.getSolutions()[0].getObjAt(0);
        REQUIRE(obj != 0.0);
        for (int i = 0; i < repetitions; i++) {
            auto otherTwoOpt{TwoOpt()};
            otherTwoOpt.setProblem(tsp);
            otherTwoOpt.run();
            auto nSolution = otherTwoOpt.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions()[0].getObjAt(0) != 0.0);
        }
    }
}