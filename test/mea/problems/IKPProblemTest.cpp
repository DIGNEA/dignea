//
// Created by amarrero on 9/3/21.
//

#include <dignea/mea/problems/IKPProblem.h>
#include <dignea/problems/KPNR.h>

#include <cmath>
#include <nlohmann/json.hpp>
#include <numeric>

#include "catch2/catch.hpp"

using json = nlohmann::json;

TEST_CASE("IKPProblem can be created", "[IKPProblem]") {
    int dimension = 100;
    shared_ptr<IKPProblem> knp = nullptr;

    SECTION("Creating a default object") {
        knp = make_shared<IKPProblem>();
        REQUIRE_FALSE(knp == nullptr);
        REQUIRE(knp->getNumberOfVars() == 1);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getNumberOfCons() == 0);
        REQUIRE_NOTHROW(knp->readData(""));
    }

    SECTION("Creating a object with all parameters") {
        int lower = 1;
        int upper = 100;
        int numberOfInstances = 100;
        knp = make_shared<IKPProblem>(dimension, numberOfInstances, lower,
                                      upper, lower, upper);
        REQUIRE_FALSE(knp == nullptr);
    }

    SECTION("Checking Getters and Setters") {
        int lower = 1;
        int upper = 100;
        int numberOfInstances = 100;
        knp = make_shared<IKPProblem>(dimension, numberOfInstances, lower,
                                      upper, lower, upper);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE(knp->getNumberOfCons() == 0);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getLowProfit() == lower);
        REQUIRE(knp->getUpWeight() == upper);
        REQUIRE(knp->getLowProfit() == lower);
        REQUIRE(knp->getUpWeight() == upper);
        REQUIRE(knp->getGeneratedInstances() == 0);
        knp->setGeneratedInstances(100);
        REQUIRE(knp->getGeneratedInstances() == 100);
        upper = 5000;
        lower = 50;
        knp->setUpWeight(upper);
        knp->setLowWeight(lower);
        knp->setUpProfit(upper);
        knp->setLowProfit(lower);
        REQUIRE(knp->getUpWeight() == upper);
        REQUIRE(knp->getLowWeight() == lower);
        REQUIRE(knp->getUpProfit() == upper);
        REQUIRE(knp->getLowProfit() == lower);
    }

    SECTION("Getting Knapsack Problem Name") {
        knp = make_shared<IKPProblem>(dimension);
        REQUIRE(knp->getName() == "IKPProblem");
    }

    SECTION("Checking limits") {
        int lowerW = 1;
        int upperW = 100;
        int lowerP = 50;
        int upperP = 300;
        int numberOfInstances = 100;
        knp = make_shared<IKPProblem>(dimension, numberOfInstances, lowerW,
                                      upperW, lowerP, upperP);
        for (int i = 0; i < dimension; i++) {
            if (i % 2 == 0) {
                REQUIRE(knp->getLowerLimit(i) == lowerW);
                REQUIRE(knp->getUpperLimit(i) == upperW);
            } else {
                REQUIRE(knp->getLowerLimit(i) == lowerP);
                REQUIRE(knp->getUpperLimit(i) == upperP);
            }
        }
        REQUIRE_THROWS(knp->getUpperLimit(-1));
        REQUIRE_THROWS(knp->getUpperLimit(dimension + 1));
        REQUIRE_THROWS(knp->getLowerLimit(-1));
        REQUIRE_THROWS(knp->getLowerLimit(dimension + 1));
    }

    SECTION("Checking optimisation direction") {
        knp = make_shared<IKPProblem>(dimension);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE_THROWS(knp->getOptimizationDirection(-1));
        REQUIRE_THROWS(knp->getOptimizationDirection(2));
    }

    SECTION("Checking create solution") {
        knp = make_shared<IKPProblem>(dimension);
        IKPSolution solution = knp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dimension * 2);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
        REQUIRE_NOTHROW(knp->evaluate(solution));
        REQUIRE(knp->evaluateConstraints(solution));
    }

    SECTION("Creating a population of solutions") {
        int populationSize = 100;
        knp = make_shared<IKPProblem>(dimension);
        vector<IKPSolution> individuals = knp->createSolutions(populationSize);
        int i = 1;
        for (const IKPSolution &solution : individuals) {
            REQUIRE(solution.getNumberOfVars() == dimension * 2);
            REQUIRE(solution.getNumberOfObjs() == 1);
            REQUIRE(solution.getNumberOfCons() == 0);
            vector<int> variables = solution.getVariables();
            float totalWeights = 0;
            /**
             * Los pesos van en las posiciones pares del vector
             */
            for (unsigned int j = 0; j < variables.size(); j += 2) {
                totalWeights += variables[j];
            }
            // int capacity = (int)0.8 * totalWeights;
            REQUIRE(solution.getCapacity() == 0);
            i++;
        }
    }

    SECTION("Creating solution without args") {
        int populationSize = 100;
        knp = make_shared<IKPProblem>(dimension);
        knp->setNumberOfInstances(populationSize);

        vector<IKPSolution> individuals(populationSize);
        for (int i = 1; i <= populationSize; i++) {
            IKPSolution solution = knp->createSolution();
            REQUIRE(solution.getNumberOfVars() == dimension * 2);
            REQUIRE(solution.getNumberOfObjs() == 1);
            REQUIRE(solution.getNumberOfCons() == 0);
            REQUIRE(solution.getCapacity() == 0);
        }
    }

    SECTION("Creating solution without args using ParallelPRNG") {
        int populationSize = 100;
        knp = make_shared<IKPProblem>(dimension);
        knp->setNumberOfInstances(populationSize);
        vector<IKPSolution> individuals(populationSize);
        ParallelPRNG random;
        for (int i = 1; i <= populationSize; i++) {
            IKPSolution solution = knp->createSolution(random);
            REQUIRE(solution.getNumberOfVars() == dimension * 2);
            REQUIRE(solution.getNumberOfObjs() == 1);
            REQUIRE(solution.getNumberOfCons() == 0);
            REQUIRE(solution.getCapacity() == 0);
        }
    }

    SECTION("IKPProblem can generated KNP problem instance") {
        knp = make_shared<IKPProblem>(dimension);
        IKPSolution solution = knp->createSolution();
        auto KPNr = knp->genOptProblem(solution);
        REQUIRE(KPNr.get());
        REQUIRE(KPNr->getNumberOfVars() == knp->getNumberOfVars() / 2);
        REQUIRE(KPNr->getNumberOfVars() * 2 == solution.getNumberOfVars());
        REQUIRE(KPNr->getCapacity() == solution.getCapacity());
        vector<int> variables = solution.getVariables();
        vector<int> profits = KPNr->getProfits();
        vector<int> weights = KPNr->getWeights();
        int knpIndex = 0;
        for (unsigned int i = 0; i < variables.size(); i++) {
            if (i % 2 == 0) {  // Weights
                REQUIRE(weights[knpIndex] == variables[i]);
            } else {
                REQUIRE(profits[knpIndex] == variables[i]);
                // Actualizamos el indice porque ya hemos visto un par (w, p)
                knpIndex++;
            }
        }
    }
}
