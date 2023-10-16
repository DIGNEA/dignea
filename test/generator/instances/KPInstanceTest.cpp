//
// Created by amarrero on 9/3/21.
//

#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>  // std::stringstream
#include <string>   // std::string

#include <catch2/catch_all.hpp>

using json = nlohmann::json;

TEST_CASE("IKPSolutions can be created", "[IKPSolutions]") {
    int nVars = 30;
    int nObjs = 1;
    int nCons = 0;
    KPInstance solution(nVars);

    SECTION("Checking not isReducedSpace") {
        REQUIRE_FALSE(solution.isReducedSpace());
    }

    SECTION("Checking isReducedSpace") {
        KPInstance solRed(nVars, 1, true);
        REQUIRE(solRed.isReducedSpace());
    }

    SECTION("Creating an empty Solution") {
        KPInstance emptySolution;
        REQUIRE(emptySolution.getNumberOfVars() == 0);
        REQUIRE(emptySolution.getNumberOfCons() == 0);
        REQUIRE(emptySolution.getNumberOfObjs() == 0);
        REQUIRE(emptySolution.getFitness() == 0.0);
        REQUIRE(emptySolution.getVariables().empty());
        REQUIRE(emptySolution.getObjectives().empty());
        REQUIRE(emptySolution.getConstraints().empty());
        REQUIRE_FALSE(emptySolution.isReducedSpace());
    }

    SECTION("Creating a Solution from pointer") {
        unique_ptr<KPInstance> pointerSolution = make_unique<KPInstance>(100);
        pointerSolution->setCapacity(3e1);
        KPInstance copy(pointerSolution.get());
        REQUIRE(pointerSolution->getNumberOfVars() == copy.getNumberOfVars());
        REQUIRE(pointerSolution->getNumberOfCons() == copy.getNumberOfCons());
        REQUIRE(pointerSolution->getNumberOfObjs() == copy.getNumberOfObjs());
        REQUIRE(pointerSolution->getFitness() == copy.getFitness());
        REQUIRE_FALSE(pointerSolution->isReducedSpace());
    }

    SECTION("Changing the capacity") {
        REQUIRE(solution.getCapacity() == 0);
        solution.setCapacity(100);
        REQUIRE(solution.getCapacity() == 100);
    }

    SECTION("Copying a Solution") {
        KPInstance copySolution(solution);
        REQUIRE(copySolution.getNumberOfVars() == solution.getNumberOfVars());
        REQUIRE(copySolution.getNumberOfObjs() == solution.getNumberOfObjs());
        REQUIRE(copySolution.getNumberOfCons() == solution.getNumberOfCons());
        REQUIRE(copySolution.getFitness() == solution.getFitness());
        REQUIRE(copySolution.getVariables() == solution.getVariables());
        REQUIRE(copySolution.getObjectives() == solution.getObjectives());
        REQUIRE(copySolution.getConstraints() == solution.getConstraints());
        REQUIRE(copySolution.getCapacity() == solution.getCapacity());
        REQUIRE_FALSE(copySolution.isReducedSpace());
    }

    SECTION("Accessing members") {
        solution.setNumberOfVars(30);
        REQUIRE(solution.getNumberOfVars() == 30);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
        REQUIRE(solution.getFitness() == 0.0);
        REQUIRE(solution.getVarAt(0) == 0);
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
        int newVariable = 25;
        vector<int> variables(nVars);
        iota(variables.begin(), variables.end(), 0);
        solution.setVarAt(0, newVariable);
        REQUIRE(solution.getVarAt(0) == newVariable);
        // Cuando actualizamos todas las variables
        solution.setVariables(variables);
        REQUIRE(solution.getVariables() == variables);

        SECTION("Going out-of-range") {
            // Cuando accedemos a un objetivo que no existe salta un error
            REQUIRE_THROWS(solution.setVarAt(nVars + 2, newVariable));
            REQUIRE_THROWS(solution.getVarAt(nVars + 2));
        }
    }

    SECTION("Updating the variables of a solution - Out-of-Range") {
        vector variables{
            0,   1, 0,  10,  10, 1, 10, 10, 50,  1, 20, 30,
            100, 0, 80, 120, 50, 1, 20, 30, 100, 0, 80, 120,
        };
        REQUIRE_THROWS(solution.setVariables(variables));
    }

    SECTION("KPInstance to_json") {
        auto dim = 100;
        auto knp = make_shared<KPDomain>(dim);
        KPInstance solution = knp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dim);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
        REQUIRE(solution.getCapacity() == 0);
        json features = solution.to_json();
        REQUIRE(features["n_vars"] == dim);
        REQUIRE(features["diversity"] == 0.0f);
        REQUIRE(features["biasedFitness"] == 0.0f);
        REQUIRE(features["fitness"] == 0.0f);
        REQUIRE(features["conf_fitness"].is_array());
        REQUIRE(features["features"].is_object());
        REQUIRE_FALSE(features["isReducedSpace"]);
        REQUIRE_FALSE(features["features"].contains("x0"));
        REQUIRE_FALSE(features["features"].contains("x1"));
    }

    SECTION("KPInstance reduced space to_json") {
        auto dim = 100;
        auto knp = make_shared<KPDomain>(dim, 1, 10, 1, 1000, 1, 1000, true);
        KPInstance solution = knp->createSolution();
        REQUIRE(solution.getNumberOfVars() == dim);
        REQUIRE(solution.getNumberOfObjs() == 1);
        REQUIRE(solution.getNumberOfCons() == 0);
        REQUIRE(solution.getCapacity() == 0);
        json features = solution.to_json();
        REQUIRE(features["isReducedSpace"]);
        REQUIRE(features["n_vars"] == dim);
        REQUIRE(features["diversity"] == 0.0f);
        REQUIRE(features["biasedFitness"] == 0.0f);
        REQUIRE(features["fitness"] == 0.0f);
        REQUIRE(features["conf_fitness"].is_array());
        REQUIRE(features["features"].is_object());
        REQUIRE(features["features"]["x0"] == -1.0);
        REQUIRE(features["features"]["x1"] == -1.0);
    }
}