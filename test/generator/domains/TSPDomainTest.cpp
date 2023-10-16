/**
 * @file TSPDomainTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-03-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/generator/domains/TSPDomain.h>
#include <dignea/generator/instances/TSPInstance.h>
#include <dignea/problems/TSP.h>

#include <cmath>
#include <nlohmann/json.hpp>
#include <numeric>

#include <catch2/catch_all.hpp>

using json = nlohmann::json;

TEST_CASE("TSPDomain can be created", "[TSPDomain]") {
    int dimension = 100;
    int objs = 1;
    SECTION("Creating a default object") {
        auto tsp = make_unique<TSPDomain>();
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE(tsp->getNumberOfVars() == 1);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfCons() == 0);
        REQUIRE(tsp->getUpperLimit(0) == 1000);
        REQUIRE(tsp->getLowerLimit(0) == 0);
        REQUIRE(tsp->getEPS() == 5.0f);
        REQUIRE(tsp->getMinNeighbors() == 2);
    }

    SECTION("Creating a object with all parameters") {
        int lower = 1;
        int upper = 100;
        auto tsp = make_unique<TSPDomain>(dimension, objs, upper, lower);
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE(tsp->getNumberOfVars() == dimension);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfCons() == 0);
        REQUIRE(tsp->getEPS() == 5.0f);
        REQUIRE(tsp->getMinNeighbors() == 2);
    }

    SECTION("Cloning a ITSP problem") {
        int lower = 1;
        int upper = 100;
        auto tsp = make_unique<TSPDomain>(dimension, objs, upper, lower);
        auto cloned = make_unique<TSPDomain>(tsp.get());
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE_FALSE(cloned == nullptr);
        REQUIRE(cloned->getNumberOfVars() == tsp->getNumberOfVars());
        REQUIRE(cloned->getNumberOfObjs() == tsp->getNumberOfObjs());
        REQUIRE(cloned->getNumberOfCons() == tsp->getNumberOfCons());
        REQUIRE(cloned->getUpperLimit(0) == tsp->getUpperLimit(0));
        REQUIRE(cloned->getLowerLimit(0) == tsp->getLowerLimit(0));
        REQUIRE(cloned->getEPS() == tsp->getEPS());
        REQUIRE(cloned->getMinNeighbors() == tsp->getMinNeighbors());
        REQUIRE(tsp->getUpperLimit(0) == upper);
        REQUIRE(tsp->getLowerLimit(0) == lower);
        REQUIRE(tsp->getEPS() == 5.0f);
        REQUIRE(tsp->getMinNeighbors() == 2);
    }

    SECTION("ITSP does nothing before evals") {
        int lower = 1;
        int upper = 100;
        auto tsp = make_unique<TSPDomain>(dimension, objs, upper, lower);
        vector<TSPInstance> solutions{};
        REQUIRE_NOTHROW(tsp->beforeEvaluation(solutions));
    }

    SECTION("Checking Getters and Setters") {
        int lower = 1;
        int upper = 100;
        auto tsp = make_unique<TSPDomain>(dimension, objs, upper, lower);

        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE(tsp->getNumberOfCons() == 0);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getUpperLimit(0) == upper);
        REQUIRE(tsp->getLowerLimit(0) == lower);
        REQUIRE(tsp->getEPS() == 5.0f);
        REQUIRE(tsp->getMinNeighbors() == 2);

        upper = 5000;
        lower = 50;
        tsp->setUpperLimit(upper);
        tsp->setLowerLimit(lower);
        REQUIRE(tsp->getUpperLimit(0) == upper);
        REQUIRE(tsp->getLowerLimit(0) == lower);

        auto newEPS = 50.0;
        auto newN = 100;
        tsp->setEPS(newEPS);
        tsp->setMinNeighbors(newN);
        REQUIRE(tsp->getEPS() == newEPS);
        REQUIRE(tsp->getMinNeighbors() == newN);
    }

    SECTION("Getting TSP Problem Name") {
        auto tsp = make_unique<TSPDomain>(dimension);
        REQUIRE(tsp->getName() == "TSPDomain");
    }

    SECTION("Checking limits") {
        int upper = 100;
        int lower = 50;
        auto tsp = make_unique<TSPDomain>(dimension, objs, upper, lower);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(tsp->getLowerLimit(i) == lower);
            REQUIRE(tsp->getUpperLimit(i) == upper);
        }
        REQUIRE_THROWS(tsp->getUpperLimit(-1));
        REQUIRE_THROWS(tsp->getUpperLimit(dimension + 1));
        REQUIRE_THROWS(tsp->getLowerLimit(-1));
        REQUIRE_THROWS(tsp->getLowerLimit(dimension + 1));
    }

    SECTION("Checking optimisation direction") {
        auto tsp = make_unique<TSPDomain>(dimension);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE_THROWS(tsp->getOptimizationDirection(-1));
        REQUIRE_THROWS(tsp->getOptimizationDirection(2));
    }

    SECTION("Checking create solution") {
        auto tsp = make_unique<TSPDomain>(dimension);
        TSPInstance solution = tsp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dimension);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
        // TODO: Include more tests
    }

    SECTION("Creating a population of solutions") {
        int populationSize = 100;
        auto tsp = make_unique<TSPDomain>(dimension);
        vector<TSPInstance> individuals = tsp->createSolutions(populationSize);
        for (const TSPInstance &solution : individuals) {
            REQUIRE(solution.getNumberOfVars() == dimension);
            REQUIRE(solution.getNumberOfObjs() == 1);
            REQUIRE(solution.getNumberOfCons() == 0);
            vector<float> variables = solution.getVariables();
        }
    }

    SECTION("Creating solution without args") {
        int populationSize = 100;
        auto tsp = make_unique<TSPDomain>(dimension);

        for (int i = 0; i < populationSize; i++) {
            TSPInstance solution = tsp->createSolution();
            REQUIRE(solution.getNumberOfVars() == dimension);
            REQUIRE(solution.getNumberOfObjs() == 1);
            REQUIRE(solution.getNumberOfCons() == 0);
            REQUIRE(tsp->evaluateConstraints(solution));
            REQUIRE_NOTHROW(tsp->evaluate(solution));
        }
    }

    SECTION("Creating solution without args using ParallelPRNG") {
        int populationSize = 100;
        auto tsp = make_unique<TSPDomain>(dimension);
        ParallelPRNG random;
        for (int i = 0; i < populationSize; i++) {
            TSPInstance solution = tsp->createSolution(random);
            REQUIRE(solution.getNumberOfVars() == dimension);
            REQUIRE(solution.getNumberOfObjs() == 1);
            REQUIRE(solution.getNumberOfCons() == 0);
        }
    }

    SECTION("TSPDomain can generated tsp problem instance") {
        auto eTSP = make_unique<TSPDomain>(dimension);
        TSPInstance solution = eTSP->createSolution();
        auto tsp = eTSP->genOptProblem(solution);
        REQUIRE(tsp.get());
        // Tenemos la mitad de variables dado que el vector se guarda flattened
        REQUIRE(tsp->getNumberOfVars() == eTSP->getNumberOfVars() / 2);
        REQUIRE_FALSE(tsp->getDistances().empty());
    }

    SECTION("TSPDomain afterEvaluation") {
        auto eTSP = make_unique<TSPDomain>(10);
        auto solution{TSPInstance(10)};
        auto expectedLength = 7;
        solution.setVariables(
            {1.0, 1.0, 2.0, 2.0, 3.0, 3.0, 4.0, 4.0, 5.0, 5.0});
        vector sols{solution};
        eTSP->afterEvaluation(sols);
        vector f = sols[0].getFeatures();
        REQUIRE_FALSE(f.empty());
        REQUIRE(f.size() == (unsigned int)expectedLength);
        REQUIRE(f[3] == (15.0 / 5));
        REQUIRE(f[4] == (15.0 / 5));
    }

    SECTION("Test empty methods") {
        auto tsp = make_unique<TSPDomain>();
        REQUIRE_NOTHROW(tsp->readData(""));
    }
}
