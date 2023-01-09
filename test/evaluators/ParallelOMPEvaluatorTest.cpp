//
// Created by amarrero on 21/12/20.
//

//
// Created by amarrero on 15/12/20.
//

#include <omp.h>
#include <dignea/evaluators/ParallelOMPEvaluator.h>
#include <dignea/problems/Sphere.h>

#include <catch2/catch_all.hpp>

TEST_CASE("Population can be evaluated in OMP", "[PopulationEvaluator]") {
    int dimension = 30;
    int populationSize = 50;
    int nThreads = 8;
    auto sphere = make_shared<Sphere>(dimension);
    vector<FloatSolution> solutions;
    solutions.reserve(populationSize);
    unique_ptr<ParallelOMPEvaluator<FloatSolution>> evaluator =
        make_unique<ParallelOMPEvaluator<FloatSolution>>(nThreads);
    for (int i = 0; i < populationSize; i++) {
        solutions.push_back(sphere->createSolution());
    }

    SECTION("Name of OMP Evaluator") {
        string name = "Parallel OMP Population Evaluator";
        unique_ptr<ParallelOMPEvaluator<FloatSolution>> evaluator =
            make_unique<ParallelOMPEvaluator<FloatSolution>>(2);
        REQUIRE(name == evaluator->getName());
    }

    SECTION("Evaluating one at a time") {
        unique_ptr<ParallelOMPEvaluator<FloatSolution>> evaluator =
            make_unique<ParallelOMPEvaluator<FloatSolution>>(2);
        for (FloatSolution &solution : solutions) {
            evaluator->evaluate(solution, sphere.get());
            REQUIRE_FALSE(solution.getObjAt(0) == 0);
            REQUIRE(solution.getObjAt(0) != 0.0);
        }
    }

    SECTION("Entire population is evaluated with 8 threads") {
        evaluator->evaluate(solutions, sphere.get());
        for (FloatSolution &solution : solutions) {
            REQUIRE_FALSE(solution.getObjAt(0) == 0);
            REQUIRE(solution.getObjAt(0) != 0.0);
        }
    }

    SECTION("Evaluating from 1 to MAX_THREADS - 2") {
        int maxThreads = omp_get_max_threads() - 2;
        for (int i = 0; i < maxThreads; i++) {
            unique_ptr<ParallelOMPEvaluator<FloatSolution>> evaluator =
                make_unique<ParallelOMPEvaluator<FloatSolution>>(i);
            evaluator->evaluate(solutions, sphere.get());
            for (FloatSolution &solution : solutions) {
                REQUIRE_FALSE(solution.getObjAt(0) == 0);
                REQUIRE(solution.getObjAt(0) != 0.0);
            }
        }
    }
}