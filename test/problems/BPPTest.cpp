/**
 * @file BPPTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * Test cases for the BPP class
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/problems/BPP.h>
#include <dignea/utilities/Comparator.h>

#include <catch2/catch_all.hpp>
#include <cmath>
#include <nlohmann/json.hpp>
#include <random>

using json = nlohmann::json;

TEST_CASE("BPP can be created", "[BPP]") {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 500);
    auto generator = [&dist, &gen]() { return dist(gen); };

    // Generate a sample of 50 items to test
    auto dimension = 50;
    auto sampleQ = 200;
    vector<int> sampleItems(dimension, 0);
    std::generate(sampleItems.begin(), sampleItems.end(), generator);

    SECTION("Creating a BPP without args") {
        unique_ptr<BPP> bpp = nullptr;
        bpp = make_unique<BPP>();
        CHECK(bpp);
        REQUIRE(bpp->getOptimizationDirection(0) == Minimize);
        REQUIRE(bpp->getNumberOfCons() == 0);
        REQUIRE(bpp->getNumberOfObjs() == 1);
    }

    SECTION("Creating BPP with all data") {
        vector items = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int dim = items.size();
        auto q = 2;
        auto bpp = make_unique<BPP>(dim, q, items);

        CHECK(bpp);
        REQUIRE(bpp->getNumberOfObjs() == 1);
        REQUIRE(bpp->getNumberOfVars() == dim);
        REQUIRE(bpp->getNumberOfCons() == 0);
        REQUIRE(bpp->getItems() == items);
        REQUIRE(bpp->getCapacity() == q);
    }

    SECTION("Clone the BPP Problem") {
        vector items = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int dim = items.size();
        auto q = 2;
        auto bpp = make_unique<BPP>(dim, q, items);

        CHECK(bpp);
        REQUIRE(bpp->getNumberOfObjs() == 1);
        REQUIRE(bpp->getNumberOfVars() == dim);
        REQUIRE(bpp->getNumberOfCons() == 0);
        REQUIRE(bpp->getItems() == items);
        REQUIRE(bpp->getCapacity() == q);

        auto bppClone = make_unique<BPP>(*bpp);

        CHECK(bppClone);
        REQUIRE(bppClone->getNumberOfVars() == bpp->getNumberOfVars());
        REQUIRE(bppClone->getNumberOfObjs() == bpp->getNumberOfObjs());
        REQUIRE(bppClone->getNumberOfCons() == bpp->getNumberOfCons());
        REQUIRE(bppClone->getItems() == items);
        REQUIRE(bppClone->getCapacity() == q);
    }

    SECTION("Checking Getters and Setters") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        REQUIRE(bpp->getOptimizationDirection(0) == Minimize);
        REQUIRE(bpp->getNumberOfCons() == 0);
        REQUIRE(bpp->getNumberOfObjs() == 1);
        REQUIRE_FALSE(bpp->getCapacity() == 0);
        REQUIRE(bpp->getCapacity() == sampleQ);
        REQUIRE_FALSE(bpp->getItems().empty());
        REQUIRE(bpp->getItems() == sampleItems);

        auto newCapacity = 100;
        bpp->setCapacity(newCapacity);
        REQUIRE(bpp->getCapacity() == newCapacity);
    }

    SECTION("Getting Bin-Packing Problem Name") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        REQUIRE(bpp->getName() == "Bin-Packing Problem");
    }

    SECTION("Checking upper limits") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(bpp->getUpperLimit(i) == dimension - 1);
        }
        REQUIRE_THROWS(bpp->getUpperLimit(-1));
        REQUIRE_THROWS(bpp->getUpperLimit(dimension + 1));
    }

    SECTION("Checking lower limits") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        for (int i = 0; i < dimension; i++) {
            REQUIRE(bpp->getLowerLimit(i) == 0);
        }
        REQUIRE_THROWS(bpp->getLowerLimit(-1));
        REQUIRE_THROWS(bpp->getLowerLimit(dimension + 1));
    }

    SECTION("Checking optimisation direction") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        REQUIRE(bpp->getOptimizationDirection(0) == Minimize);
        REQUIRE_THROWS(bpp->getOptimizationDirection(-1));
        REQUIRE_THROWS(bpp->getOptimizationDirection(2));
    }

    SECTION("Checking create solution") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        IntIntSolution solution = bpp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dimension);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
    }

    SECTION("Checking evaluate solution") {
        auto bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        IntIntSolution solution = bpp->createSolution();

        bpp->evaluate(solution);
        REQUIRE(solution.getObjAt(0) != 0);
        REQUIRE(solution.getFitness() != 0);
    }

    SECTION("Check evaluates return 0") {
        auto dimension = 6;
        vector items = {4, 6, 2, 5, 3, 7};  // Item sizes
        auto q = 10;                        // Maximum capacity of each bin
        vector binAssignment = {0, 0, 1, 1, 2, 2};

        auto bpp = make_unique<BPP>(dimension, q, items);
        IntIntSolution solution = bpp->createSolution();
        solution.setVariables(binAssignment);

        bpp->evaluate(solution);

        REQUIRE(solution.getObjAt(0) == 3);
    }

    SECTION("Check evaluates with larger instance that return 1 bin") {
        auto dimension = 9;
        vector items = {8, 3, 6, 2, 7, 4, 5, 1, 9};  // Item sizes
        auto q = 10;  // Maximum capacity of each bin
        vector binAssignment = {0, 1, 1, 2, 0, 1, 2, 0, 1};

        auto bpp = make_unique<BPP>(dimension, q, items);
        IntIntSolution solution = bpp->createSolution();
        solution.setVariables(binAssignment);

        bpp->evaluate(solution);

        REQUIRE(solution.getObjAt(0) == 3);
    }

    SECTION("Check evaluates with larger instance that uses 5 bins") {
        auto dimension = 9;
        vector items = {7, 3, 5, 4, 2, 6, 8, 1, 9};
        auto q = 10;  // Maximum capacity of each bin
        vector binAssignment = {0, 0, 1, 1, 2, 2, 3, 3, 4};

        auto bpp = make_unique<BPP>(dimension, q, items);
        IntIntSolution solution = bpp->createSolution();
        solution.setVariables(binAssignment);

        bpp->evaluate(solution);

        REQUIRE(solution.getObjAt(0) == 5);
        REQUIRE(solution.getFitness() == 0.814f);
    }

    SECTION("Read data throws exception") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        std::string path = "/MYPATH";
        REQUIRE_THROWS(bpp->readData(path));
    }

    SECTION("Read data works perfectly") {
        unique_ptr<BPP> bpp = nullptr;

        bpp = make_unique<BPP>(dimension, sampleQ, sampleItems);
        std::string path = "../data/instances/BinPacking/sample.bpp";
        REQUIRE_NOTHROW(bpp->readData(path));
    }

    SECTION("Creating BPP from data works perfectly") {
        vector<int> expectedItems = {
            275, 386, 129, 835, 900, 664, 230, 850, 456, 893, 35,  227, 641,
            162, 431, 321, 682, 113, 390, 565, 904, 696, 671, 819, 974, 231,
            586, 227, 270, 506, 612, 842, 95,  254, 496, 62,  915, 221, 371,
            171, 922, 839, 373, 216, 855, 662, 485, 602, 745, 338};

        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_unique<BPP>(path);
        REQUIRE_FALSE(bpp == nullptr);
        REQUIRE(bpp->getCapacity() == 400);
        REQUIRE(bpp->getNumberOfVars() == 50);
        REQUIRE(bpp->getItems() == expectedItems);
    }

    SECTION("Solving a BPP from data works perfectly") {
        vector<int> expectedItems = {
            275, 386, 129, 835, 900, 664, 230, 850, 456, 893, 35,  227, 641,
            162, 431, 321, 682, 113, 390, 565, 904, 696, 671, 819, 974, 231,
            586, 227, 270, 506, 612, 842, 95,  254, 496, 62,  915, 221, 371,
            171, 922, 839, 373, 216, 855, 662, 485, 602, 745, 338};

        std::string path = "../data/instances/BinPacking/sample.bpp";
        auto bpp = make_unique<BPP>(path);
        auto solution = bpp->createSolution();
        bpp->evaluate(solution);

        REQUIRE_FALSE(bpp == nullptr);
        REQUIRE(bpp->getCapacity() == 400);
        REQUIRE(bpp->getNumberOfVars() == 50);
        REQUIRE(bpp->getItems() == expectedItems);
        REQUIRE(solution.getObjAt(0) != 0);
        REQUIRE(solution.getFitness() != 0);
        REQUIRE(solution.getObjAt(0) == bpp->getNumberOfVars());
    }

    SECTION("BPP to_json format") {
        vector items = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int dim = items.size();
        auto q = 2;
        auto bpp = make_unique<BPP>(dim, q, items);

        CHECK(bpp);
        REQUIRE(bpp->getNumberOfObjs() == 1);
        REQUIRE(bpp->getNumberOfVars() == dim);
        REQUIRE(bpp->getNumberOfCons() == 0);
        REQUIRE(bpp->getItems() == items);
        REQUIRE(bpp->getCapacity() == q);
        json j = bpp->to_json();
        REQUIRE(j["capacity"] == q);
        REQUIRE(j["items"] == items);
    }
}