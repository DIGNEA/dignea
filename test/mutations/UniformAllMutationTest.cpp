//
// Created by amarrero on 21/12/20.
//

#include <dignea/mutations/UniformAllMutation.h>
#include <dignea/problems/Sphere.h>

#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Testing UniformAllMutation", "[UniformAllMutation]") {
    int dimension = 10;
    double mutationRate = 0.5;
    SECTION("Testing UniformMutationAll with double solutions") {
        auto sphere = make_unique<Sphere>(dimension);
        FloatSolution solution = sphere->createSolution();
        vector<float> vars = solution.getVariables();
        // Comprobamos que son iguales
        REQUIRE(solution.getVariables() == vars);
        // Mutamos
        auto mutation = make_unique<UniformAllMutation<FloatSolution>>();
        mutation->run(solution, mutationRate, sphere.get());
        REQUIRE_FALSE(solution.getVariables() == vars);
    }

    SECTION(
        "Testing UniformMutationAll with double solutions and unique_ptr "
        "Problem") {
        auto sphere = make_unique<Sphere>(dimension);
        FloatSolution solution = sphere->createSolution();
        vector<float> vars = solution.getVariables();
        // Comprobamos que son iguales
        REQUIRE(solution.getVariables() == vars);
        // Mutamos
        auto mutation = make_unique<UniformAllMutation<FloatSolution>>();
        mutation->run(solution, mutationRate, sphere.get());
        REQUIRE_FALSE(solution.getVariables() == vars);
    }

    SECTION("Uniform All Mutation getName") {
        auto mutation = make_unique<UniformAllMutation<FloatSolution>>();
        REQUIRE(mutation->getName() == "Uniform All Mutation");
    }
}