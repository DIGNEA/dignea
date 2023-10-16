#include <dignea/algorithms/tsp_heuristics/NearestNeighbour.h>

#include <string>

#include <catch2/catch_all.hpp>
using namespace tsp_heuristics;
using namespace std;
using json = nlohmann::json;

TEST_CASE(
    "Tests for the NN TSP Heuristic"
    "[NN]") {
    SECTION("NearestNeighbour can be instantiated") {
        auto nn = NearestNeighbour();
        REQUIRE(nn.getName() == "NearestNeighbour TSP");
        REQUIRE(nn.getID() == "NN TSP");
    }

    SECTION("Get json info") {
        auto nn{NearestNeighbour()};
        REQUIRE(nn.to_json()["name"] == "NearestNeighbour TSP");
    }

    SECTION("NearestNeighbour can solve a TSP instance") {
        auto nn{NearestNeighbour()};
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
        auto nn{NearestNeighbour()};
        nn.setProblem(knp);
        REQUIRE_NOTHROW(nn.run());
        auto solution = nn.getResults();
        for (int i = 0; i < repetitions; i++) {
            auto otherNN{NearestNeighbour()};
            otherNN.setProblem(knp);
            otherNN.run();
            auto nSolution = otherNN.getResults();
            REQUIRE(nSolution.getNumOfSolutions() ==
                    solution.getNumOfSolutions());
            REQUIRE(nSolution.getSolutions() == solution.getSolutions());
            REQUIRE(nSolution.to_json() == solution.to_json());
        }
    }
}