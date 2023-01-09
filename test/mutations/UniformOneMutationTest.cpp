//
// Created by amarrero on 21/12/20.
//

#include <catch2/catch_all.hpp>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/problems/Sphere.h>

TEST_CASE("Testing UniformOneMutation", "[UniformOneMutation]") {
    int dimension = 30;
    double mutationRate = 1.0;
    SECTION("Testing UniformOneMutation with double solutions") {
        auto sphere = make_shared<Sphere>(dimension);
        FloatSolution solution = sphere->createSolution();
        vector<float> vars = solution.getVariables();
        // Comprobamos que son iguales
        REQUIRE(solution.getVariables() == vars);
        // Mutamos
        auto mutation = make_shared<UniformOneMutation<FloatSolution>>();
        mutation->run(solution, mutationRate, sphere.get());
        REQUIRE_FALSE(solution.getVariables() == vars);
    }

    SECTION("Testing UniformOneMutation with double solutions and unique_ptr Problem") {
        auto sphere = make_unique<Sphere>(dimension);
        FloatSolution solution = sphere->createSolution();
        vector<float> vars = solution.getVariables();
        // Comprobamos que son iguales
        REQUIRE(solution.getVariables() == vars);
        // Mutamos
        auto mutation = make_shared<UniformOneMutation<FloatSolution>>();
        mutation->run(solution, mutationRate, sphere.get());
        REQUIRE_FALSE(solution.getVariables() == vars);
    }

    SECTION("Uniform One Mutation getName") {
        auto mutation = make_shared<UniformOneMutation<FloatSolution>>();
        REQUIRE(mutation->getName() == "Uniform One Mutation");
    }
}