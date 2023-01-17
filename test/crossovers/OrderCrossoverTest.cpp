/**
 * @file OrderCrossoverTest.cpp
 * @author Alejandro Marrero
 * @brief
 * @version 0.1
 * @date 2022-03-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <dignea/crossovers/OrderCrossover.h>
#include <dignea/generator/domains/TSPDomain.h>
#include <dignea/generator/instances/TSPInstance.h>
#include <dignea/problems/TSP.h>

#include <vector>

#include <catch2/catch_all.hpp>

using namespace std;

TEST_CASE("Testing OrderCrossover", "[OrderCrossover]") {
    SECTION("Testing OrderCrossover with double solutions") {
        std::string path = "../data/instances/TSP/djibouti.tsp";
        shared_ptr<Problem<IntFloatSolution>> tsp = make_shared<TSP>(path);

        auto solution1 = tsp->createSolution();
        auto solution2 = tsp->createSolution();
        vector vars1 = solution1.getVariables();
        vector vars2 = solution2.getVariables();

        // Comprobamos que son iguales
        REQUIRE(solution1.getVariables() == vars1);
        REQUIRE(solution2.getVariables() == vars2);
        REQUIRE_FALSE(solution1.getVariables() == solution2.getVariables());
        // StartCity está duplicada
        REQUIRE(vars1.size() == (unsigned int)tsp->getNumberOfVars() + 1);
        REQUIRE(vars2.size() == (unsigned int)tsp->getNumberOfVars() + 1);

        // Cruzamos
        auto crossover = make_shared<OrderCrossover<>>();
        crossover->run(solution1, solution2);
        REQUIRE_FALSE(solution1.getVariables() == vars1);
        REQUIRE_FALSE(solution2.getVariables() == vars2);
        REQUIRE_FALSE(solution1.getVariables() == solution2.getVariables());
        REQUIRE(solution1.getVariables().size() ==
                (unsigned int)tsp->getNumberOfVars() + 1);
        REQUIRE(solution2.getVariables().size() ==
                (unsigned int)tsp->getNumberOfVars() + 1);

        // Comprobamos que están todos
        vars1 = solution1.getVariables();
        vars2 = solution2.getVariables();
        sort(vars1.begin(), vars1.end());
        sort(vars2.begin(), vars2.end());
        REQUIRE(vars1[0] == 0);
        REQUIRE(vars1[1] == 0);
        REQUIRE(vars2[0] == 0);
        REQUIRE(vars2[1] == 0);
        for (unsigned int i = 1; i < vars1.size() - 1; i++) {
            REQUIRE(vars1[i] < vars1[i + 1]);
            REQUIRE(vars2[i] < vars2[i + 1]);
        }
    }

    SECTION("Testing OrderCrossover with random solutions") {
        auto dim = 500;  // 250 cities
        auto etsp = make_unique<TSPDomain>(dim);
        auto etsps{TSPInstance(dim)};
        auto tsp = etsp->genOptProblem(etsps);
        auto solution1 = tsp->createSolution();
        auto solution2 = tsp->createSolution();
        vector vars1 = solution1.getVariables();
        vector vars2 = solution2.getVariables();

        // Comprobamos que son iguales
        REQUIRE(solution1.getVariables() == vars1);
        REQUIRE(solution2.getVariables() == vars2);
        REQUIRE_FALSE(solution1.getVariables() == solution2.getVariables());
        // StartCity está duplicada
        REQUIRE(vars1.size() == (unsigned int)tsp->getNumberOfVars() + 1);
        REQUIRE(vars2.size() == (unsigned int)tsp->getNumberOfVars() + 1);

        // Cruzamos
        auto crossover = make_shared<OrderCrossover<>>();
        crossover->run(solution1, solution2);
        REQUIRE_FALSE(solution1.getVariables() == vars1);
        REQUIRE_FALSE(solution2.getVariables() == vars2);
        REQUIRE_FALSE(solution1.getVariables() == solution2.getVariables());
        REQUIRE(solution1.getVariables().size() ==
                (unsigned int)tsp->getNumberOfVars() + 1);
        REQUIRE(solution2.getVariables().size() ==
                (unsigned int)tsp->getNumberOfVars() + 1);

        // Comprobamos que están todos
        vars1 = solution1.getVariables();
        vars2 = solution2.getVariables();
        sort(vars1.begin(), vars1.end());
        sort(vars2.begin(), vars2.end());
        REQUIRE(vars1[0] == 0);
        REQUIRE(vars1[1] == 0);
        REQUIRE(vars2[0] == 0);
        REQUIRE(vars2[1] == 0);
        for (unsigned int i = 1; i < vars1.size() - 1; i++) {
            REQUIRE(vars1[i] < vars1[i + 1]);
            REQUIRE(vars2[i] < vars2[i + 1]);
        }
    }

    SECTION("Order Crossover getName") {
        auto crossover = make_shared<OrderCrossover<>>();
        REQUIRE(crossover->getName() == "Order Crossover");
    }
}