//
// Created by amarrero on 10/12/20.
//

#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>
#include <dignea/problems/Sphere.h>

#include "catch2/catch.hpp"

TEST_CASE("Problems can be created", "[Sphere]") {
    int nVars = 30;
    auto problem = make_unique<Sphere>(nVars);
    SECTION("Creating a Problem with default constructor") {
        auto defProblem = make_unique<Sphere>();
        REQUIRE(defProblem.get() != nullptr);
    }

    SECTION("Accessing variables") {
        REQUIRE(problem->getNumberOfVars() == 30);
    }

    SECTION("Accessing objs") { REQUIRE(problem->getNumberOfObjs() == 1); }

    SECTION("Accessing const") { REQUIRE(problem->getNumberOfCons() == 0); }

    SECTION("Getting Name") { REQUIRE(problem->getName() == "Sphere"); }

    SECTION("Acessing upper limits") {
        REQUIRE(problem->getUpperLimit(0) == 5.12f);
    }

    SECTION("Acessing lower limits") {
        REQUIRE(problem->getLowerLimit(0) == -5.12f);
    }

    SECTION("Acessing upper limits out of range") {
        // Cuando accedemos a un objetivo que no existe salta un error
        REQUIRE_THROWS(problem->getLowerLimit(nVars + 2));
    }

    SECTION("Acessing lower limits out of range") {
        REQUIRE_THROWS(problem->getUpperLimit(nVars + 2));
    }

    SECTION("Accessing the optimisation direction") {
        REQUIRE(problem->getOptimizationDirection(0) == Minimize);
    }

    SECTION("Accessing the optimisation direction false") {
        REQUIRE_FALSE(problem->getOptimizationDirection(0) == Maximize);
    }

    SECTION("Going out-of-range") {
        REQUIRE_THROWS(problem->getOptimizationDirection(nVars + 2));
    }

    SECTION("Creating a solution to the problem with dimension match") {
        FloatSolution solution = problem->createSolution();
        REQUIRE(solution.getNumberOfVars() == problem->getNumberOfVars());
    }

    SECTION("Creating a solution to the problem with objs match") {
        FloatSolution solution = problem->createSolution();
        REQUIRE(solution.getNumberOfObjs() == problem->getNumberOfObjs());
    }

    SECTION("Creating a solution to the problem with const match") {
        FloatSolution solution = problem->createSolution();
        REQUIRE(solution.getNumberOfCons() == problem->getNumberOfCons());
    }

    SECTION("Creating a solution to the problem with fitness 0") {
        FloatSolution solution = problem->createSolution();
        REQUIRE(solution.getFitness() == 0.0);
    }
}