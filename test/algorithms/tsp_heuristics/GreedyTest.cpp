#include <dignea/algorithms/tsp_heuristics/Greedy.h>

#include <string>

#include <catch2/catch_all.hpp>
using namespace tsp_heuristics;
using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the Greedy TSP Heuristic"
    "[Greedy TSP]") {
    SECTION("Greedy can be instantiated") {
        auto nn = Greedy();
        REQUIRE(nn.getName() == "Greedy TSP");
        REQUIRE(nn.getID() == "Greedy TSP");
    }

    SECTION("Get json info") {
        auto nn{Greedy()};
        REQUIRE(nn.to_json()["name"] == "Greedy TSP");
    }

    SECTION("Greedy can solve a TSP instance") {
        auto nn{Greedy()};
        std::string path = "../data/instances/TSP/example1.tsp";
        auto knp = make_shared<TSP>(path);
        REQUIRE_FALSE(knp == nullptr);
        nn.setProblem(knp);
        REQUIRE_NOTHROW(nn.run());
        REQUIRE_FALSE(nn.getResults().empty());
    }

    SECTION("Evaluating the determinism of the algorithm") {
        std::string path = "../data/instances/TSP/example1.tsp";
        auto knp = make_shared<TSP>(path);
        auto repetitions = 9;
        auto nn{Greedy()};
        nn.setProblem(knp);
        REQUIRE_NOTHROW(nn.run());
        auto solution = nn.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherGreedy{Greedy()};
            otherGreedy.setProblem(knp);
            otherGreedy.run();
            auto nSolution = otherGreedy.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions()[0].getObjAt(0) ==
                    solution.getSolutions()[0].getObjAt(0));
        }
    }
}