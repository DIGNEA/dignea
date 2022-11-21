//
// Created by amarrero on 21/12/20.
//

#include <dignea/problems/KPNR.h>

#include <cmath>
#include <nlohmann/json.hpp>

#include "catch2/catch.hpp"

using json = nlohmann::json;

TEST_CASE("KPNR tests", "[KPNR]") {
    int dimension = 50;
    shared_ptr<KPNR> knp = nullptr;
    SECTION("Creating a KNP empty") {
        knp = make_unique<KPNR>();
        CHECK(knp);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getCapacity() == 0);
        REQUIRE(knp->getProfits().empty());
        REQUIRE(knp->getWeights().empty());
    }

    SECTION("Creating a object with only the dimension") {
        knp = make_unique<KPNR>(dimension);
        REQUIRE_FALSE(knp == nullptr);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getCapacity() != 0);
        REQUIRE_FALSE(knp->getProfits().empty());
        REQUIRE_FALSE(knp->getWeights().empty());
    }

    SECTION("Creating a object with all parameters") {
        int lower = 1;
        int upper = 100;
        knp = make_unique<KPNR>(dimension, lower, upper, lower, upper);
        REQUIRE_FALSE(knp == nullptr);
    }

    SECTION("Checking Getters and Setters") {
        int lower = 1;
        int upper = 100;
        knp = make_unique<KPNR>(dimension, lower, upper, lower, upper);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE_FALSE(knp->getCapacity() == 0);
        REQUIRE_FALSE(knp->getProfits().empty());
        REQUIRE_FALSE(knp->getWeights().empty());
        REQUIRE(knp->getLowProfit() == lower);
        REQUIRE(knp->getUpWeight() == upper);
        REQUIRE(knp->getLowProfit() == lower);
        REQUIRE(knp->getUpWeight() == upper);
        knp->setCapacity(upper);
        REQUIRE(knp->getCapacity() == upper);
        vector<int> weights(dimension, 50);
        vector<int> profits(dimension, 80);
        knp->setProfits(profits);
        REQUIRE(knp->getProfits() == profits);
        knp->setWeights(weights);
        REQUIRE(knp->getWeights() == weights);
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
        knp = make_unique<KPNR>(dimension);
        REQUIRE(knp->getName() == "Knapsack Problem");
    }

    SECTION("Checking upper limits") {
        knp = make_unique<KPNR>(dimension);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(knp->getUpperLimit(i) == 1);
        }
        REQUIRE_THROWS(knp->getUpperLimit(-1));
        REQUIRE_THROWS(knp->getUpperLimit(dimension + 1));
    }

    SECTION("Checking lower limits") {
        knp = make_unique<KPNR>(dimension);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(knp->getLowerLimit(i) == 0);
        }
        REQUIRE_THROWS(knp->getLowerLimit(-1));
        REQUIRE_THROWS(knp->getLowerLimit(dimension + 1));
    }

    SECTION("Checking optimisation direction") {
        knp = make_unique<KPNR>(dimension);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE_THROWS(knp->getOptimizationDirection(-1));
        REQUIRE_THROWS(knp->getOptimizationDirection(2));
    }

    SECTION("Checking create solution") {
        knp = make_unique<KPNR>(dimension);
        BoolFloatSolution solution = knp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dimension);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 1);
    }

    SECTION("Checking create solution with ParallelPRNG") {
        knp = make_unique<KPNR>(dimension);
        ParallelPRNG random;
        BoolFloatSolution solution = knp->createSolution(random);
        REQUIRE(solution.getNumberOfVars() == dimension);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 1);
    }

    SECTION("Checking evaluate solution") {
        int lower = 1;
        int upper = 100;
        knp = make_unique<KPNR>(dimension, lower, upper, lower, upper);
        BoolFloatSolution solution = knp->createSolution();
        vector<char> vars(dimension, false);
        solution.setVariables(vars);
        knp->evaluate(solution);
        REQUIRE_FALSE(solution.getObjAt(0) == -1);
    }

    SECTION("Read data throws exception") {
        knp = make_unique<KPNR>(dimension);
        std::string path = "/MYPATH";
        REQUIRE_THROWS(knp->readData(path));
    }

    SECTION("Read data works perfectly") {
        knp = make_unique<KPNR>(dimension);
        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        REQUIRE_NOTHROW(knp->readData(path));
    }

    SECTION("Creating KP from data works perfectly") {
        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        knp = make_unique<KPNR>(path);
        REQUIRE_FALSE(knp == nullptr);
    }

    SECTION("Cloning KP problem") {
        KPNR knp(dimension);
        KPNR copyKP(knp);
        REQUIRE(copyKP.getNumberOfVars() == knp.getNumberOfVars());
        REQUIRE(copyKP.getNumberOfObjs() == knp.getNumberOfObjs());
        REQUIRE(copyKP.getNumberOfCons() == knp.getNumberOfCons());
        REQUIRE(copyKP.getCapacity() == knp.getCapacity());
        REQUIRE(copyKP.getWeights() == knp.getWeights());
        REQUIRE(copyKP.getProfits() == knp.getProfits());
        REQUIRE(copyKP.getLowWeight() == knp.getLowWeight());
        REQUIRE(copyKP.getUpWeight() == knp.getUpWeight());
        REQUIRE(copyKP.getLowProfit() == knp.getLowProfit());
        REQUIRE(copyKP.getUpProfit() == knp.getUpProfit());
    }
}