//
// Created by amarrero on 13/4/21.
//

#include <dignea/core/Front.h>
#include <dignea/problems/DoubleSphere.h>
#include <dignea/problems/ZDT.h>
#include <dignea/types/SolutionTypes.h>

#include <catch2/catch_all.hpp>
#include <vector>

using namespace std;

TEST_CASE("Front tests", "[Front]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);
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
        FloatSolution newSolution(nVars, 1, constraints);
        newSolution.setVariables(vars);
        newSolution.setObjectives({10.0});
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

    SECTION("Adding a 2D solution") {
        auto front = make_unique<Front<FloatSolution>>(solutions);
        REQUIRE_THROWS(front->addSolution(solutions[0]));
    }

    SECTION("Adding a 1D solution using addSolution with problem") {
        auto front = make_unique<Front<FloatSolution>>(solutions);
        auto problem = make_unique<ZDT>(ZDT_MODE::ZDT1, 30);
        FloatSolution newSolution(nVars, 1, constraints);
        newSolution.setVariables(vars);
        newSolution.setObjectives({10.0});
        REQUIRE_NOTHROW(front->addSolution(newSolution, problem.get()));
    }

    SECTION("Adding a 2D solution using addSolution with problem") {
        auto front = make_unique<Front<FloatSolution>>(0);
        auto problem = make_unique<ZDT>(ZDT_MODE::ZDT1, 30);
        for (const FloatSolution &solution : solutions) {
            REQUIRE_NOTHROW(front->addSolution(solution, problem.get()));
        }
        // They should be the same
        REQUIRE(front->getNumOfSolutions() == 1);
    }
}