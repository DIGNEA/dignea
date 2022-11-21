//
// Created by amarrero on 25/6/21.
//

#include <dignea/problems/Rastrigin.h>

#include <nlohmann/json.hpp>

#include "catch2/catch.hpp"

using json = nlohmann::json;

TEST_CASE("Rastrigin can be evaluated", "[Rastrigin]") {
    int dimension = 30;
    auto problem = make_unique<Rastrigin>(dimension);

    SECTION("Evaluating a optimal solution - unique_ptr") {
        FloatSolution optimal = problem->createSolution();
        for (int i = 0; i < dimension; i++) {
            optimal.setVarAt(i, 0.0);
        }
        problem->evaluate(optimal);
        REQUIRE(optimal.getObjAt(0) == 0.0);
    }

    SECTION("No data reading allowed") {
        REQUIRE_THROWS(problem->readData("NOTHING"));
    }

    SECTION("EvaluatContraints not implemented") {
        FloatSolution optimal = problem->createSolution();
        REQUIRE_THROWS(problem->evaluateConstraints(optimal));
    }

    SECTION("Clone the Rastrigin Problem") {
        auto RastriginCopy = make_unique<Rastrigin>(problem.get());
        REQUIRE(RastriginCopy != nullptr);
        REQUIRE(RastriginCopy->getNumberOfVars() == problem->getNumberOfVars());
        REQUIRE(RastriginCopy->getNumberOfObjs() == problem->getNumberOfObjs());
        REQUIRE(RastriginCopy->getNumberOfCons() == problem->getNumberOfCons());
    }

    SECTION("Rastrigin Name") { REQUIRE(problem->getName() == "Rastrigin"); }

    SECTION("Rastrigin to_json") {
        json data = problem->to_json();
        REQUIRE(data["name"] == problem->getName());
        REQUIRE(data["num_objs"] == problem->getNumberOfObjs());
        REQUIRE(data["num_vars"] == problem->getNumberOfVars());
        REQUIRE(data["num_cons"] == problem->getNumberOfCons());
        REQUIRE(data["up_limit"] == problem->getUpperLimit(0));
        REQUIRE(data["low_limit"] == problem->getLowerLimit(0));
    }

    SECTION("Check Rastrigin OptimizationDirection") {
        REQUIRE(problem->getOptimizationDirection(0) == Minimize);
        REQUIRE_THROWS(problem->getOptimizationDirection(-1));
        REQUIRE_THROWS(problem->getOptimizationDirection(1));
    }

    SECTION("Check Rastrigin Upper Limits") {
        for (int i = 0; i < dimension; i++) {
            REQUIRE(problem->getUpperLimit(i) == 5.12f);
        }
    }

    SECTION("Check Rastrigin Lower Limits") {
        for (int i = 0; i < dimension; i++) {
            REQUIRE(problem->getLowerLimit(i) == -5.12f);
        }
    }

    SECTION("Check Rastrigin Upper Limits throws") {
        REQUIRE_THROWS(problem->getUpperLimit(-1) == 5.12);
        REQUIRE_THROWS(problem->getUpperLimit(dimension * 10) == 5.12);
    }

    SECTION("Check Rastrigin Lower Limits throws") {
        REQUIRE_THROWS(problem->getLowerLimit(-1) == 5.12);
        REQUIRE_THROWS(problem->getLowerLimit(dimension * 10) == 5.12);
    }

    SECTION("Creating a Solution with the Rastrigin Problem") {
        FloatSolution solution = problem->createSolution();
        REQUIRE(solution.getNumberOfVars() == problem->getNumberOfVars());
        REQUIRE(solution.getNumberOfObjs() == problem->getNumberOfObjs());
        REQUIRE(solution.getNumberOfCons() == problem->getNumberOfCons());
    }

    SECTION("Check bounds in all dimensions") {
        FloatSolution solution = problem->createSolution();
        for (int i = 0; i < problem->getNumberOfVars(); i++) {
            REQUIRE_FALSE(solution.getVarAt(i) < -5.12);
            REQUIRE_FALSE(solution.getVarAt(i) > 5.12);
        }
    }

    SECTION(
        "Creating a Solution with the Rastrigin Problem and ParalllelPRNG") {
        ParallelPRNG random;
        FloatSolution solution = problem->createSolution(random);
        REQUIRE(solution.getNumberOfVars() == problem->getNumberOfVars());
        REQUIRE(solution.getNumberOfObjs() == problem->getNumberOfObjs());
        REQUIRE(solution.getNumberOfCons() == problem->getNumberOfCons());
        for (int i = 0; i < problem->getNumberOfVars(); i++) {
            REQUIRE_FALSE(solution.getVarAt(i) < -5.12);
            REQUIRE_FALSE(solution.getVarAt(i) > 5.12);
        }
    }
}