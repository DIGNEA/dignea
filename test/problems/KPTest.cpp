//
// Created by amarrero on 21/12/20.
//

#include <dignea/problems/KP.h>

#include <cmath>
#include <nlohmann/json.hpp>

#include <catch2/catch_all.hpp>

using json = nlohmann::json;

TEST_CASE("KP can be created", "[KP]") {
    int dimension = 50;

    SECTION("Creating a KP without args") {
        unique_ptr<KP> knp = nullptr;
        knp = make_unique<KP>();
        CHECK(knp);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getNumberOfObjs() == 1);
    }

    SECTION("Creating a object with only the dimension") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        REQUIRE_FALSE(knp == nullptr);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getCapacity() != 0);
        REQUIRE_FALSE(knp->getProfits().empty());
        REQUIRE_FALSE(knp->getWeights().empty());
    }

    SECTION("Creating a object with all parameters") {
        unique_ptr<KP> knp = nullptr;
        int lower = 1;
        int upper = 100;
        knp = make_unique<KP>(dimension, lower, upper, lower, upper);
        REQUIRE_FALSE(knp == nullptr);
    }

    SECTION("Creating KP with all data") {
        vector weights = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        vector profits = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto dim = 10;
        auto q = 1000;
        auto knp = make_unique<KP>(dim, q, weights, profits);
        CHECK(knp);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getNumberOfVars() == dim);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getProfits() == profits);
        REQUIRE(knp->getWeights() == weights);
        REQUIRE(knp->getCapacity() == q);
    }

    SECTION("Clone the KP Problem") {
        vector weights = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        vector profits = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto dim = 10;
        auto q = 1000;
        auto knp = make_unique<KP>(dim, q, weights, profits);
        CHECK(knp);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getNumberOfVars() == dim);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getProfits() == profits);
        REQUIRE(knp->getWeights() == weights);
        REQUIRE(knp->getCapacity() == q);

        auto knpClone = make_unique<KP>(*knp);

        CHECK(knpClone);
        REQUIRE(knpClone->getNumberOfVars() == knp->getNumberOfVars());
        REQUIRE(knpClone->getNumberOfObjs() == knp->getNumberOfObjs());
        REQUIRE(knpClone->getNumberOfCons() == knp->getNumberOfCons());
        REQUIRE(knpClone->getProfits() == profits);
        REQUIRE(knpClone->getWeights() == weights);
        REQUIRE(knpClone->getCapacity() == q);
    }

    SECTION("Checking Getters and Setters") {
        unique_ptr<KP> knp = nullptr;

        int lower = 1;
        int upper = 100;
        knp = make_unique<KP>(dimension, lower, upper, lower, upper);
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
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        REQUIRE(knp->getName() == "Knapsack Problem");
    }

    SECTION("Checking upper limits") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(knp->getUpperLimit(i) == 1);
        }
        REQUIRE_THROWS(knp->getUpperLimit(-1));
        REQUIRE_THROWS(knp->getUpperLimit(dimension + 1));
    }

    SECTION("Checking lower limits") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(knp->getLowerLimit(i) == 0);
        }
        REQUIRE_THROWS(knp->getLowerLimit(-1));
        REQUIRE_THROWS(knp->getLowerLimit(dimension + 1));
    }

    SECTION("Checking optimisation direction") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        REQUIRE(knp->getOptimizationDirection(0) == Maximize);
        REQUIRE_THROWS(knp->getOptimizationDirection(-1));
        REQUIRE_THROWS(knp->getOptimizationDirection(2));
    }

    SECTION("Checking create solution") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        BoolFloatSolution solution = knp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dimension);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 1);
    }

    SECTION("Checking create solution with ParallelPRNG") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        ParallelPRNG random;
        BoolFloatSolution solution = knp->createSolution(random);
        REQUIRE(solution.getNumberOfVars() == dimension);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 1);
    }

    SECTION("Checking evaluate solution") {
        unique_ptr<KP> knp = nullptr;

        int lower = 1;
        int upper = 100;
        knp = make_unique<KP>(dimension, lower, upper, lower, upper);
        BoolFloatSolution solution = knp->createSolution();
        vector<char> vars(dimension, false);
        solution.setVariables(vars);
        knp->evaluate(solution);
        REQUIRE_FALSE(solution.getObjAt(0) == -1);
    }

    SECTION("Read data throws exception") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        std::string path = "/MYPATH";
        REQUIRE_THROWS(knp->readData(path));
    }

    SECTION("Read data works perfectly") {
        unique_ptr<KP> knp = nullptr;

        knp = make_unique<KP>(dimension);
        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        REQUIRE_NOTHROW(knp->readData(path));
    }

    SECTION("Creating KP from data works perfectly") {
        unique_ptr<KP> knp = nullptr;

        std::string path =
            "../data/instances/KnapsackProblem/AlmostStronglyCorrelated/"
            "AlmostStronglyCorrelated_N_50_R_1000_1.kp";
        knp = make_unique<KP>(path);
        REQUIRE_FALSE(knp == nullptr);
    }

    SECTION("KP to_json format") {
        vector weights = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        vector profits = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto dim = 10;
        auto q = 1000;
        auto upLimit = 1000;
        auto lowLimit = 1;
        auto knp = make_unique<KP>(dim, q, weights, profits);
        knp->setUpProfit(upLimit);
        knp->setUpWeight(upLimit);
        knp->setLowWeight(lowLimit);
        knp->setLowProfit(lowLimit);
        CHECK(knp);
        REQUIRE(knp->getNumberOfObjs() == 1);
        REQUIRE(knp->getNumberOfVars() == dim);
        REQUIRE(knp->getNumberOfCons() == 1);
        REQUIRE(knp->getProfits() == profits);
        REQUIRE(knp->getWeights() == weights);
        REQUIRE(knp->getCapacity() == q);
        json j = knp->to_json();
        REQUIRE(j["capacity"] == q);
        REQUIRE(j["profits"] == profits);
        REQUIRE(j["weights"] == weights);
        REQUIRE(j["low_profit"] == lowLimit);
        REQUIRE(j["up_profit"] == upLimit);
        REQUIRE(j["low_weight"] == lowLimit);
        REQUIRE(j["up_weight"] == upLimit);
    }
}