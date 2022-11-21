
/**
 * @file TSPTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief TSP test cases
 * @version 0.1
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <dignea/algorithms/singleobjective/SteadyGA.h>
#include <dignea/builders/GABuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/crossovers/OrderCrossover.h>
#include <dignea/problems/TSP.h>

#include <cmath>
#include <memory>
#include <nlohmann/json.hpp>

#include "catch2/catch.hpp"

using namespace std;
using json = nlohmann::json;

TEST_CASE("TSP can be created", "[TSP]") {
    const int dimension = 100;
    const int evals = 1e5;

    SECTION("Creating an TSP object") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>();
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE(tsp->getNumberOfCons() == 1);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfVars() == 0);
        REQUIRE(tsp->getDistances().size() == 0u);
        REQUIRE(tsp->getPoints().size() == 0u);
        REQUIRE(tsp->getPathToInstance() == "");
    }

    SECTION("Creating an TSP object with dimension") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE(tsp->getNumberOfCons() == 1);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfVars() == dimension);
        REQUIRE(tsp->getDistances().size() == 0u);
        REQUIRE(tsp->getPoints().size() == 0u);
        REQUIRE(tsp->getPathToInstance() == "");
    }

    SECTION("Creating an TSP object with dimension and coords") {
        unique_ptr<TSP> tsp = nullptr;
        vector<coords> coordinates{
            {0.0, 1.0},  {0.0, 10.0},  {10.0, 1.0},  {10.0, 10.0},
            {50.0, 1.0}, {20.0, 30.0}, {100.0, 0.0}, {80.0, 120.0},
        };
        tsp = make_unique<TSP>(dimension, coordinates);
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE(tsp->getNumberOfCons() == 1);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfVars() == dimension);
        REQUIRE(tsp->getDistances().size() == dimension);
        REQUIRE(tsp->getPoints() == coordinates);
        REQUIRE(tsp->getPathToInstance() == "");
    }

    SECTION("Creating a object from other") {
        unique_ptr<TSP> tsp = nullptr;
        unique_ptr<TSP> tspWithDim = make_unique<TSP>(50);
        tsp = make_unique<TSP>(tspWithDim.get());
        REQUIRE(tsp->getNumberOfVars() == tspWithDim->getNumberOfVars());
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE(tsp->getNumberOfCons() == 1);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfVars() == 50);
        REQUIRE(tsp->getDistances().size() == 0u);
        REQUIRE(tsp->getPoints().size() == 0u);
        REQUIRE(tsp->getPathToInstance() == "");
    }

    SECTION("TSP to_json format") {
        vector<coords> coordinates{
            {0.0, 1.0},  {0.0, 10.0},  {10.0, 1.0},  {10.0, 10.0},
            {50.0, 1.0}, {20.0, 30.0}, {100.0, 0.0}, {80.0, 120.0},
        };
        unique_ptr<TSP> tsp = make_unique<TSP>(dimension, coordinates);
        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE(tsp->getNumberOfCons() == 1);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfVars() == dimension);
        REQUIRE(tsp->getDistances().size() == dimension);
        REQUIRE(tsp->getPoints() == coordinates);
        REQUIRE(tsp->getPathToInstance() == "");
        json j = tsp->to_json();
        REQUIRE(j["points"] == coordinates);
        REQUIRE(j["distances"] == tsp->getDistances());
    }

    SECTION("Checking Getters and Setters") {
        unique_ptr<TSP> tsp = nullptr;
        tsp = make_unique<TSP>(dimension);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE(tsp->getNumberOfCons() == 1);
        REQUIRE(tsp->getNumberOfObjs() == 1);
        REQUIRE(tsp->getNumberOfVars() == dimension);
    }

    SECTION("Getting TSP Problem Name") {
        unique_ptr<TSP> tsp = nullptr;
        tsp = make_unique<TSP>(dimension);
        REQUIRE(tsp->getName() == "TSP");
    }

    SECTION("Checking upper limits") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(tsp->getUpperLimit(i) == tsp->getNumberOfVars() - 1);
        }
        REQUIRE_THROWS(tsp->getUpperLimit(-1));
        REQUIRE_THROWS(tsp->getUpperLimit(dimension + 1));
    }

    SECTION("Checking lower limits") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(tsp->getLowerLimit(i) == 1);
        }
        REQUIRE_THROWS(tsp->getLowerLimit(-1));
        REQUIRE_THROWS(tsp->getLowerLimit(dimension + 1));
    }

    SECTION("Checking optimisation direction") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        REQUIRE(tsp->getOptimizationDirection(0) == Minimize);
        REQUIRE_THROWS(tsp->getOptimizationDirection(-1));
        REQUIRE_THROWS(tsp->getOptimizationDirection(2));
    }

    SECTION("Checking create solution") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        IntFloatSolution solution = tsp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dimension + 1);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 1);
        REQUIRE(tsp->getDistances().size() == 0u);
        REQUIRE(tsp->getPoints().size() == 0u);
        REQUIRE(tsp->getPathToInstance() == "");

        vector vars = solution.getVariables();
        REQUIRE(vars.size() == (unsigned int)tsp->getNumberOfVars() + 1);
        sort(vars.begin(), vars.end());
        REQUIRE(vars[0] == 0);
        REQUIRE(vars[1] == 0);
        for (unsigned int i = 2; i < vars.size() - 1; i++) {
            REQUIRE(vars[i] < vars[i + 1]);
        }
    }

    SECTION("TSP Create solution using PRGN throws") {
        auto tsp = make_unique<TSP>(dimension);
        auto prng = ParallelPRNG();
        REQUIRE_THROWS(tsp->createSolution(prng));
    }

    SECTION("Checking create solution with instance") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        auto tsp = make_unique<TSP>(path);
        IntFloatSolution solution = tsp->createSolution();
        REQUIRE(solution.getNumberOfVars() == tsp->getNumberOfVars() + 1);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 1);
        REQUIRE(tsp->getDistances().size() == 38u);
        REQUIRE(tsp->getPoints().size() == 38u);
        REQUIRE(tsp->getPathToInstance() == path);

        vector vars = solution.getVariables();
        REQUIRE(vars.size() == (unsigned int)tsp->getNumberOfVars() + 1);
        sort(vars.begin(), vars.end());
        REQUIRE(vars[0] == 0);
        REQUIRE(vars[1] == 0);
        for (unsigned int i = 2; i < vars.size() - 1; i++) {
            REQUIRE(vars[i] < vars[i + 1]);
        }
    }

    SECTION("Checking evaluate solution throws") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        IntFloatSolution solution = tsp->createSolution();
        vector<int> vars(dimension + 1, 0);  // TSP includes one more
        solution.setVariables(vars);

        REQUIRE_THROWS(tsp->evaluate(solution));
    }

    SECTION("Read data throws exception") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        std::string path = "/MYPATH";
        REQUIRE_THROWS(tsp->readData(path));
    }

    SECTION("Read data works perfectly") {
        unique_ptr<TSP> tsp = nullptr;

        tsp = make_unique<TSP>(dimension);
        std::string path = "../data/instances/TSP/djibouti.tsp";
        REQUIRE_NOTHROW(tsp->readData(path));
    }

    SECTION("Creating TSP from data works perfectly") {
        unique_ptr<TSP> tsp = nullptr;

        std::string path = "../data/instances/TSP/djibouti.tsp";
        tsp = make_unique<TSP>(path);
        REQUIRE_FALSE(tsp == nullptr);
    }

    SECTION("Evaluating no feasible TSP solution") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        auto tsp = make_unique<TSP>(path);
        REQUIRE_FALSE(tsp == nullptr);

        IntFloatSolution solution = tsp->createSolution();
        vector<int> vars(tsp->getNumberOfVars() + 1, 1);
        solution.setVariables(vars);

        REQUIRE_NOTHROW(tsp->evaluate(solution));
        REQUIRE(solution.getFitness() == 0.0f);
    }

    SECTION("Evaluating generated solution") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        auto tsp = make_unique<TSP>(path);
        IntFloatSolution solution = tsp->createSolution();

        REQUIRE_FALSE(tsp == nullptr);
        REQUIRE_NOTHROW(tsp->evaluate(solution));
        REQUIRE(solution.getFitness() != std::numeric_limits<float>::max());
    }

    SECTION("Evaluating modified solution") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        auto tsp = make_unique<TSP>(path);
        auto cx = make_unique<OrderCrossover<>>();
        auto mut = make_unique<SwapMutation<IntFloatSolution>>();
        auto parent1 = tsp->createSolution();
        auto parent2 = tsp->createSolution();

        REQUIRE(tsp->evaluateConstraints(parent1) == true);
        REQUIRE(tsp->evaluateConstraints(parent2) == true);

        cx->run(parent1, parent2);
        REQUIRE(tsp->evaluateConstraints(parent1) == true);
        REQUIRE(tsp->evaluateConstraints(parent2) == true);

        mut->run(parent1, 0, tsp.get());
        mut->run(parent2, 0, tsp.get());
        REQUIRE(tsp->evaluateConstraints(parent1) == true);
        REQUIRE(tsp->evaluateConstraints(parent2) == true);

        REQUIRE_NOTHROW(tsp->evaluate(parent1));
        REQUIRE_NOTHROW(tsp->evaluate(parent2));
    }

    SECTION("Checking distance matrix") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        auto tsp = make_unique<TSP>(path);
        auto distMatrix = tsp->getDistances();

        REQUIRE_FALSE(tsp == nullptr);
        for (int i = 0; i < tsp->getNumberOfVars(); i++) {
            for (int j = 0; j < tsp->getNumberOfVars(); j++) {
                REQUIRE(distMatrix[i][j] == distMatrix[j][i]);
                if (i == j) {
                    REQUIRE(distMatrix[i][j] == 0);
                }
            }
        }
    }

    SECTION("SGA can solve the TSP") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        auto cx = make_unique<OrderCrossover<>>();
        unique_ptr<AbstractGA<IntFloatSolution>> ga =
            GABuilder<IntFloatSolution>::create(GAType::Steady)
                .toSolve(make_unique<TSP>(path))
                .with()
                .populationOf(32)
                .with()
                .mutation(MutType::SwapMutation)
                .crossover(move(cx))
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(evals);

        ga->run();
        REQUIRE(ga->getPerformedEvaluations() >= ga->getMaxEvaluations());
        REQUIRE_FALSE(ga->getElapsedTime() == 0.0);
        Front<IntFloatSolution> solutions = ga->getResults();
        REQUIRE(solutions.getNumOfSolutions() >= 1);
        for (auto& s : solutions.getSolutions()) {
            REQUIRE(s.getConstraintCoeff() == 0);
            REQUIRE(s.getFitness() >= 0);
        }
    }

    SECTION("ParGA can solve the TSP") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        unique_ptr<ParallelGeneticAlgorithm<IntFloatSolution>> ga =
            ParGABuilder<IntFloatSolution>::create()
                .toSolve(make_unique<TSP>(path))
                .with()
                .populationOf(32)
                .with()
                .mutation(MutType::SwapMutation)
                .crossover(CXType::Order)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(evals);

        ga->run();
        REQUIRE(ga->getPerformedEvaluations() >= ga->getMaxEvaluations());
        REQUIRE_FALSE(ga->getElapsedTime() == 0.0);
        Front<IntFloatSolution> solutions = ga->getResults();
        REQUIRE(solutions.getNumOfSolutions() >= 1);
        for (auto& s : solutions.getSolutions()) {
            REQUIRE(s.getConstraintCoeff() == 0);
            REQUIRE(s.getFitness() >= 0);
        }
    }

    SECTION("FIGA can solve the TSP") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        unique_ptr<AbstractGA<IntFloatSolution>> ga =
            GABuilder<IntFloatSolution>::create(GAType::FIGA)
                .toSolve(make_unique<TSP>(path))
                .with()
                .populationOf(32)
                .with()
                .mutation(MutType::SwapMutation)
                .crossover(CXType::Order)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(evals);

        ga->run();
        REQUIRE(ga->getPerformedEvaluations() >= ga->getMaxEvaluations());
        REQUIRE_FALSE(ga->getElapsedTime() == 0.0);
        Front<IntFloatSolution> solutions = ga->getResults();
        REQUIRE(solutions.getNumOfSolutions() >= 1);
        for (auto& s : solutions.getSolutions()) {
            REQUIRE(s.getConstraintCoeff() == 0);
            REQUIRE(s.getFitness() >= 0);
        }
    }

    SECTION("Generational can solve the TSP") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        unique_ptr<AbstractGA<IntFloatSolution>> ga =
            GABuilder<IntFloatSolution>::create(GAType::Generational)
                .toSolve(make_unique<TSP>(path))
                .with()
                .populationOf(32)
                .with()
                .mutation(MutType::SwapMutation)
                .crossover(CXType::Order)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(evals);

        ga->run();
        REQUIRE(ga->getPerformedEvaluations() >= ga->getMaxEvaluations());
        REQUIRE_FALSE(ga->getElapsedTime() == 0.0);
        Front<IntFloatSolution> solutions = ga->getResults();
        REQUIRE(solutions.getNumOfSolutions() >= 1);
        for (auto& s : solutions.getSolutions()) {
            REQUIRE(s.getConstraintCoeff() == 0);
            REQUIRE(s.getFitness() >= 0);
        }
    }
}