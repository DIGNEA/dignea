
/**
 * @file SwapMutationTest.cpp
 * @author Alejandro Marrero
 * @brief
 * @version 0.1
 * @date 2022-03-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <dignea/mutations/SwapMutation.h>
#include <dignea/problems/TSP.h>

#include <catch2/catch_all.hpp>

TEST_CASE("Testing SwapMutation", "[SwapMutation]") {
    std::string path = "../data/instances/TSP/djibouti.tsp";
    SECTION("Testing SwapMutation with integer solutions") {
        auto tsp = make_shared<TSP>(path);
        auto nSols = 10;
        auto nTries = 1000;
        for (int t = 0; t < nTries; t++) {
            for (int i = 0; i < nSols; i++) {
                auto solution = tsp->createSolution();
                vector<int> vars = solution.getVariables();
                // Comprobamos que son iguales
                REQUIRE(solution.getVariables() == vars);
                REQUIRE(vars.size() ==
                        (unsigned int)tsp->getNumberOfVars() + 1);
                // Mutamos
                auto mutation = make_shared<SwapMutation<IntFloatSolution>>();
                mutation->run(solution, 0, tsp.get());
                vector newVars = solution.getVariables();

                REQUIRE_FALSE(solution.getVariables() == vars);
                REQUIRE(newVars[0] == 0);
                REQUIRE(newVars[newVars.size() - 1] == 0);
                REQUIRE(newVars.size() ==
                        (unsigned int)tsp->getNumberOfVars() + 1);

                // No duplicates
                sort(newVars.begin(), newVars.end());
                for (unsigned int i = 1; i < newVars.size() - 1; i++) {
                    REQUIRE(newVars[i] < newVars[i + 1]);
                }
            }
        }
    }

    SECTION("Uniform One Mutation getName") {
        auto mutation = make_shared<SwapMutation<FloatSolution>>();
        REQUIRE(mutation->getName() == "Swap Mutation");
    }
}