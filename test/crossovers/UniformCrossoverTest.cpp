//
// Created by amarrero on 21/12/20.
//

#include <catch2/catch_all.hpp>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/problems/Sphere.h>
#include <vector>

using namespace std;

TEST_CASE("Testing UniformCrossover", "[UniformCrossover]") {
    int dimension = 10;

    SECTION("Testing UniformCrossover with double solutions") {
        shared_ptr<Problem<FloatSolution>> sphere = make_shared<Sphere>(
                dimension);
        FloatSolution solution1 = sphere->createSolution();
        FloatSolution solution2 = sphere->createSolution();
        vector<float> vars1 = solution1.getVariables();
        vector<float> vars2 = solution2.getVariables();

        // Comprobamos que son iguales
        REQUIRE(solution1.getVariables() == vars1);
        REQUIRE(solution2.getVariables() == vars2);

        // Cruzamos
        shared_ptr<Crossover<FloatSolution>> crossover =
                make_shared<UniformCrossover<FloatSolution>>();
        crossover->run(solution1, solution2);
        REQUIRE_FALSE(solution1.getVariables() == vars1);
        REQUIRE_FALSE(solution2.getVariables() == vars2);
    }

    SECTION("Uniform Crossover getName") {
        shared_ptr<Crossover<FloatSolution>> crossover =
                make_shared<UniformCrossover<FloatSolution>>();
        REQUIRE(crossover->getName() == "Uniform One Crossover");
    }
}