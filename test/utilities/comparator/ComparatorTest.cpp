//
// Created by amarrero on 15/12/20.
//

#include <dignea/problems/DoubleSphere.h>
#include <dignea/problems/SampleProblem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/Comparator.h>

#include <catch2/catch_all.hpp>

TEST_CASE("Comparator can be created", "[Comparator]") {
    int dimension = 30;
    int numObjectives = 2;
    float minFitness = 50;
    float maxFitness = 100;
    FloatSolution firstInd(dimension, numObjectives);
    FloatSolution secondInd(dimension, numObjectives);
    SECTION("Compare by fitness - First wins") {
        firstInd.setFitness(maxFitness);
        secondInd.setFitness(minFitness);
        REQUIRE(cmpByFitness(firstInd, secondInd) == FIRST_BEST);
    }
    SECTION("Compare by fitness - Second wins") {
        firstInd.setFitness(minFitness);
        secondInd.setFitness(maxFitness);
        REQUIRE(cmpByFitness(firstInd, secondInd) == SECOND_BEST);
    }
    SECTION("Compare by fitness - Tie") {
        firstInd.setFitness(minFitness);
        secondInd.setFitness(minFitness);
        REQUIRE(cmpByFitness(firstInd, secondInd) == TIE);
    }

    SECTION("Compare by objective - First wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(0, minFitness);
        secondInd.setObjAt(0, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 0, sample.get()) == FIRST_BEST);
    }
    SECTION("Compare by fitness - Second wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(0, maxFitness);
        secondInd.setObjAt(0, minFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 0, sample.get()) == SECOND_BEST);
    }
    SECTION("Compare by fitness - Tie") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(0, maxFitness);
        secondInd.setObjAt(0, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 0, sample.get()) == TIE);
    }

    SECTION("Compare by objective Maximise - Second Wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(1, minFitness);
        secondInd.setObjAt(1, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 1, sample.get()) == SECOND_BEST);
    }

    SECTION("Compare by objective Maximise - First Wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(1, maxFitness);
        secondInd.setObjAt(1, minFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 1, sample.get()) == FIRST_BEST);
    }

    SECTION("Compare by objective Maximise - Tie") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(1, maxFitness);
        secondInd.setObjAt(1, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 1, sample.get()) == TIE);
    }

    SECTION("Throw exception because outOfRange cmpByObj") {
        auto sample = make_shared<SampleProblem>(dimension);
        REQUIRE_THROWS(
            cmpByObj(firstInd, secondInd, numObjectives + 2, sample.get()));
    }

    SECTION("Dominance Test of two DoubleSphere solutions- First Wins") {
        int dimension = 30;
        auto problem = make_unique<DoubleSphere>(dimension);
        FloatSolution solution1 = problem->createSolution();
        FloatSolution solution2 = problem->createSolution();

        for (int i = 0; i < dimension; i++) {
            solution1.setVarAt(i, 0.0);
            solution2.setVarAt(i, 4.0);
        }
        problem->evaluate(solution1);
        problem->evaluate(solution2);
        REQUIRE(dominanceTest(solution1, solution2, problem.get()) ==
                FIRST_DOMINATES);
    }

    SECTION("Dominance Test of two DoubleSphere solutions- Second Wins") {
        int dimension = 30;
        auto problem = make_unique<DoubleSphere>(dimension);
        FloatSolution solution1 = problem->createSolution();
        FloatSolution solution2 = problem->createSolution();

        for (int i = 0; i < dimension; i++) {
            solution1.setVarAt(i, 4.0);
            solution2.setVarAt(i, 0.0);
        }
        problem->evaluate(solution1);
        problem->evaluate(solution2);
        REQUIRE(dominanceTest(solution1, solution2, problem.get()) ==
                SECOND_DOMINATES);
    }

    SECTION("Dominance Test of two DoubleSphere solutions- Equals Wins") {
        int dimension = 30;
        auto problem = make_unique<DoubleSphere>(dimension);
        FloatSolution solution1 = problem->createSolution();
        FloatSolution solution2 = problem->createSolution();

        for (int i = 0; i < dimension; i++) {
            solution1.setVarAt(i, 0.0);
            solution2.setVarAt(i, 0.0);
        }
        problem->evaluate(solution1);
        problem->evaluate(solution2);
        REQUIRE(dominanceTest(solution1, solution2, problem.get()) ==
                NON_DOMINANCES_EQUALS);
    }

    SECTION("Comparing a population") {
        auto numOfSolutions = 10;
        vector<FloatSolution> population{};
        auto problem = make_unique<DoubleSphere>(dimension);

        for (int i = 0; i < numOfSolutions; i++) {
            FloatSolution solution(10, 2);
            solution.setObjAt(0, 0);
            solution.setObjAt(1, 0.5);
            population.push_back(move(solution));
        }
        for (int i = 0; i < numOfSolutions - 1; i++) {
            for (int j = 1; j < numOfSolutions; j++) {
                REQUIRE(dominanceTest(population[i], population[j],
                                      problem.get()) == NON_DOMINANCES_EQUALS);
            }
        }
    }
}