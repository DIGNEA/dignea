//
// Created by amarrero on 21/12/20.
//

//
// Created by amarrero on 15/12/20.
//

#include <dignea/evaluators/PopulationEvaluator.h>
#include <dignea/problems/Sphere.h>

#include <catch2/catch_all.hpp>

TEST_CASE("Population can be evaluated", "[PopulationEvaluator]") {
    int dimension = 30;
    int populationSize = 50;
    auto sphere = make_shared<Sphere>(dimension);
    vector<FloatSolution> solutions;
    solutions.reserve(populationSize);
    auto evaluator = make_unique<PopulationEvaluator<FloatSolution>>();
    for (int i = 0; i < populationSize; i++) {
        solutions.push_back(sphere->createSolution());
    }
    SECTION("Entire population is evaluated") {
        evaluator->evaluate(solutions, sphere.get());
        for (FloatSolution &solution : solutions) {
            REQUIRE_FALSE(solution.getObjAt(0) == 0);
            REQUIRE(solution.getObjAt(0) != 0.0);
        }
    }

    SECTION("One solutions is evaluated") {
        FloatSolution solution = sphere->createSolution();
        evaluator->evaluate(solution, sphere.get());
        REQUIRE_FALSE(solution.getObjAt(0) == 0);
        REQUIRE(solution.getObjAt(0) != 0.0);
    }
}