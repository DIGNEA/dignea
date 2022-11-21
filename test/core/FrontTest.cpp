//
// Created by amarrero on 13/4/21.
//

#include <dignea/core/Front.h>
#include <dignea/types/SolutionTypes.h>

#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Front tests", "[Front]") {
    const int nSolutions = 30;
    const int nVars = 10;
    const int nObjs = 2;
    const int constraints = 0;
    vector objs = {1.0f, 2.5f};
    vector vars = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};

    vector<FloatSolution> solutions;
    solutions.reserve(nSolutions);
    for (int i = 0; i < nSolutions; i++) {
        FloatSolution solution(nVars, nObjs, constraints);
        solution.setVariables(vars);
        solution.setObjectives(objs);
        solution.setFitness(float(i) * 1000.0);
        solutions.push_back(solution);
    }

    SECTION("Creating a Front with only the size") {
        Front<FloatSolution> front(nSolutions);
        REQUIRE(front.getNumOfSolutions() == nSolutions);
    }

    SECTION("Creating a Front with only the size as a pointer") {
        auto front = make_unique<Front<FloatSolution>>(nSolutions);
        REQUIRE(front != nullptr);
        REQUIRE(front->getNumOfSolutions() == nSolutions);
    }

    SECTION("Creating a Front with all solutions") {
        Front<FloatSolution> front(solutions);
        REQUIRE(front.getNumOfSolutions() == nSolutions);
        REQUIRE(solutions == front.getSolutions());
    }

    SECTION("Creating a Front with only the size as a pointer") {
        auto front = make_unique<Front<FloatSolution>>(solutions);
        REQUIRE(front != nullptr);
        REQUIRE(front->getNumOfSolutions() == nSolutions);
        REQUIRE(solutions == front->getSolutions());
    }

    SECTION("Adding a solution to the Front") {
        auto front = make_unique<Front<FloatSolution>>(solutions);
        FloatSolution newSolution(nVars, nObjs, constraints);
        newSolution.setVariables(vars);
        newSolution.setObjectives(objs);
        newSolution.setFitness(100 * 1000.0);
        front->addSolution(newSolution);
        solutions.push_back(newSolution);
        REQUIRE(front->getNumOfSolutions() == nSolutions + 1);
        REQUIRE(front->getSolutions().size() == nSolutions + 1);
        REQUIRE(solutions == front->getSolutions());
    }

    SECTION("Setting the solutions") {
        auto front = make_unique<Front<FloatSolution>>(nSolutions);
        front->setSolutions(solutions);
        REQUIRE((unsigned int)front->getNumOfSolutions() == solutions.size());
        REQUIRE(solutions == front->getSolutions());
    }

    SECTION("Copying a Front") {
        auto front = make_unique<Front<FloatSolution>>(solutions);
        auto copyFront = make_unique<Front<FloatSolution>>(front.get());
        REQUIRE(front->getNumOfSolutions() == copyFront->getNumOfSolutions());
        REQUIRE(front->getSolutions() == copyFront->getSolutions());
    }
}