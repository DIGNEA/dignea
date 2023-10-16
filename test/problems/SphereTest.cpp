//
// Created by amarrero on 21/12/20.
//

#include <dignea/problems/DoubleSphere.h>
#include <dignea/problems/Sphere.h>

#include <catch2/catch_all.hpp>

TEST_CASE("Sphere can be evaluated", "[Sphere]") {
    int dimension = 30;
    auto problem = make_unique<Sphere>(dimension);

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

    SECTION("Clone the Sphere Problem") {
        auto sphereCopy = make_unique<Sphere>(problem.get());
        REQUIRE(sphereCopy != nullptr);
        REQUIRE(sphereCopy->getNumberOfVars() == problem->getNumberOfVars());
        REQUIRE(sphereCopy->getNumberOfObjs() == problem->getNumberOfObjs());
        REQUIRE(sphereCopy->getNumberOfCons() == problem->getNumberOfCons());
    }

    SECTION("Creating a Solution with the Sphere Problem") {
        FloatSolution solution = problem->createSolution();
        REQUIRE(solution.getNumberOfVars() == problem->getNumberOfVars());
        REQUIRE(solution.getNumberOfObjs() == problem->getNumberOfObjs());
        REQUIRE(solution.getNumberOfCons() == problem->getNumberOfCons());
        for (int i = 0; i < problem->getNumberOfVars(); i++) {
            REQUIRE_FALSE(solution.getVarAt(i) < -5.12);
            REQUIRE_FALSE(solution.getVarAt(i) > 5.12);
        }
    }

    SECTION("Creating a Solution with the Sphere Problem and ParalllelPRNG") {
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

    SECTION("Creation of a DoubleSphere") {
        int dimension = 30;
        auto problem = make_unique<DoubleSphere>(dimension);
        FloatSolution optimal = problem->createSolution();

        REQUIRE(problem->getNumberOfObjs() == 2);
        REQUIRE(optimal.getNumberOfObjs() == 2);
        for (auto i = 0; i < problem->getNumberOfObjs(); i++) {
            REQUIRE(problem->getLowerLimit(i) == -5.12f);
            REQUIRE(problem->getUpperLimit(i) == 5.12f);
        }

        for (int i = 0; i < dimension; i++) {
            optimal.setVarAt(i, 0.0);
        }
        problem->evaluate(optimal);
        REQUIRE(optimal.getObjAt(0) == 0.0);
        REQUIRE(optimal.getObjAt(1) == 0.0);
    }
}