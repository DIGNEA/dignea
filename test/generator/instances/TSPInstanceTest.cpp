/**
 * @file ITSPSolutionTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/generator/instances/TSPInstance.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>  // std::stringstream
#include <string>   // std::string

#include <catch2/catch_all.hpp>

using json = nlohmann::json;

TEST_CASE("ITSPSolutions can be created", "[ITSPSolutions]") {
    int nVars = 30;
    int nObjs = 1;
    int nCons = 0;
    TSPInstance solution(nVars);

    SECTION("Accesing data from instance with only dim") {
        REQUIRE(solution.getNumberOfVars() == nVars);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
        REQUIRE(solution.getFitness() == 0.0);
        REQUIRE(solution.getVarAt(0) == 0.0);
        REQUIRE(solution.getFeatures().size() == 0u);
        REQUIRE(solution.getObjectives().size() == 1u);
        REQUIRE(solution.getVariables().size() == (unsigned int)nVars);
        REQUIRE(solution.getConstraints().size() == 0u);
        REQUIRE_THROWS(solution.getVarAt(-1));
        REQUIRE_THROWS(solution.getVarAt(nVars + 10));
    }

    SECTION("Creating an empty Solution") {
        TSPInstance emptySolution;
        REQUIRE(emptySolution.getNumberOfVars() == 0);
        REQUIRE(emptySolution.getNumberOfCons() == 0);
        REQUIRE(emptySolution.getNumberOfObjs() == 0);
        REQUIRE(emptySolution.getFitness() == 0.0);
        REQUIRE(emptySolution.getVariables().empty());
        REQUIRE(emptySolution.getObjectives().empty());
        REQUIRE(emptySolution.getConstraints().empty());
        REQUIRE(solution.getFeatures().size() == 0u);
    }

    SECTION("Creating a Solution from pointer") {
        auto pointerSolution = make_unique<TSPInstance>(100);
        TSPInstance copy(pointerSolution.get());
        REQUIRE(pointerSolution->getNumberOfVars() == copy.getNumberOfVars());
        REQUIRE(pointerSolution->getNumberOfCons() == copy.getNumberOfCons());
        REQUIRE(pointerSolution->getNumberOfObjs() == copy.getNumberOfObjs());
        REQUIRE(pointerSolution->getFitness() == copy.getFitness());
        REQUIRE(solution.getFeatures().size() == 0u);
    }

    SECTION("Copying a Solution") {
        TSPInstance copySolution(solution);
        REQUIRE(copySolution.getNumberOfVars() == solution.getNumberOfVars());
        REQUIRE(copySolution.getNumberOfObjs() == solution.getNumberOfObjs());
        REQUIRE(copySolution.getNumberOfCons() == solution.getNumberOfCons());
        REQUIRE(copySolution.getFitness() == solution.getFitness());
        REQUIRE(copySolution.getVariables() == solution.getVariables());
        REQUIRE(copySolution.getObjectives() == solution.getObjectives());
        REQUIRE(copySolution.getConstraints() == solution.getConstraints());
        REQUIRE(solution.getFeatures().size() == 0u);
    }

    SECTION("Accessing members") {
        solution.setNumberOfVars(30);
        REQUIRE(solution.getNumberOfVars() == 30);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
        REQUIRE(solution.getFitness() == 0.0);
        REQUIRE(solution.getVarAt(0) == 0.0);
        REQUIRE(solution.getFeatures().size() == 0u);

        REQUIRE_THROWS(solution.getVarAt(-1));
        REQUIRE_THROWS(solution.getVarAt(nVars + 10));
    }

    SECTION("Acessing getters of a solution") {
        REQUIRE(solution.getVariables().size() == (long unsigned int)nVars);
        REQUIRE(solution.getObjectives().size() == (long unsigned int)nObjs);
        REQUIRE(solution.getConstraints().size() == (long unsigned int)nCons);
    }

    SECTION("Updating the fitness of a solution") {
        double newObjective = 50.0;
        solution.setObjAt(0, newObjective);
        REQUIRE(solution.getObjAt(0) == newObjective);
        SECTION("Going out-of-range") {
            // Cuando accedemos a un objetivo que no existe salta un error
            REQUIRE_THROWS(solution.setObjAt(1, newObjective));
            REQUIRE_THROWS(solution.getObjAt(1));
        }
    }

    SECTION("Updating the variables of a solution") {
        int nVars = 16;
        TSPInstance newSolution(nVars);
        float newVar{10.0f};
        vector<float> variables{
            0.0,  1.0, 0.0,  10.0, 10.0,  1.0, 10.0, 10.0,
            50.0, 1.0, 20.0, 30.0, 100.0, 0.0, 80.0, 120.0,
        };
        newSolution.setVarAt(0, 10.0);
        REQUIRE(newSolution.getVarAt(0) == newVar);
        // Cuando actualizamos todas las variables
        newSolution.setVariables(variables);
        REQUIRE(newSolution.getVariables() == variables);

        // Cuando accedemos a un objetivo que no existe salta un error
        REQUIRE_THROWS(newSolution.setVarAt(nVars + 2, newVar));
        REQUIRE_THROWS(newSolution.getVarAt(nVars + 2));
    }

    SECTION("Updating the variables of a solution - Out-of-Range") {
        int nVars = 16;
        TSPInstance newSolution(nVars);
        vector<float> variables{
            0.0,  1.0, 0.0,  10.0, 10.0,  1.0, 10.0, 10.0,
            50.0, 1.0, 20.0, 30.0, 100.0, 0.0, 80.0, 120.0,
            50.0, 1.0, 20.0, 30.0, 100.0, 0.0, 80.0, 120.0,
        };
        REQUIRE_THROWS(newSolution.setVariables(variables));
    }

    SECTION("TSPInstance to_json") {
        TSPInstance newSolution(16);
        vector<float> variables{
            0.0,  1.0, 0.0,  10.0, 10.0,  1.0, 10.0, 10.0,
            50.0, 1.0, 20.0, 30.0, 100.0, 0.0, 80.0, 120.0,
        };
        newSolution.setVariables(variables);
        json features = newSolution.to_json();
        REQUIRE(features["n_vars"] == 16);
        REQUIRE(features["coords"] == variables);
        REQUIRE(features["diversity"] == 0.0f);
        REQUIRE(features["biasedFitness"] == 0.0f);
        REQUIRE(features["fitness"] == 0.0f);
        REQUIRE(features["conf_fitness"].is_array());
        REQUIRE(features["features"].is_object());
    }
}