//
// Created by amarrero on 22/6/21.
//

#include <dignea/algorithms/singleobjective/SteadyGA.h>
#include <dignea/builders/GABuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/problems/Sphere.h>
#include <dignea/types/SolutionTypes.h>

#include <catch2/catch_all.hpp>

TEST_CASE("GABuilder tests", "[GABuilder]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);

    const float eps = 1e-5;

    SECTION("Creating a Steady GA") {
        auto steady = make_unique<SteadyGA<FloatSolution>>();
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::Steady)
                .toSolve(make_unique<Sphere>(100))
                .with()
                .populationOf(100)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.001)
                .withCrossRate(0.9)
                .runDuring(100);
        CHECK(steady);
        CHECK(ga);
        REQUIRE(steady->getName() == ga->getName());
        REQUIRE(ga->getMutation()->getName() == "Uniform All Mutation");
        REQUIRE(ga->getCrossover()->getName() == "Uniform One Crossover");
        REQUIRE(ga->getSelection()->getName() == "Binary Tournament Selection");
        REQUIRE(abs(ga->getMutationRate() - 0.001) <= eps);
        REQUIRE(abs(ga->getCrossRate() - 0.9) <= eps);
        REQUIRE(ga->getMaxEvaluations() == 100);
        REQUIRE(ga->getPopulationSize() == 100);
        REQUIRE(ga->getProblem()->getName() == "Sphere");
    }

    SECTION("Creating a Generational GA") {
        auto generational = make_unique<GenerationalGA<FloatSolution>>();
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::Generational)
                .toSolve(make_unique<Sphere>(100))
                .with()
                .populationOf(100)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(100);
        CHECK(generational);
        CHECK(ga);
        REQUIRE(generational->getName() == ga->getName());
        REQUIRE(ga->getMutation()->getName() == "Uniform All Mutation");
        REQUIRE(ga->getCrossover()->getName() == "Uniform One Crossover");
        REQUIRE(ga->getSelection()->getName() == "Binary Tournament Selection");
        REQUIRE(abs(ga->getMutationRate() - 0.05) <= eps);
        REQUIRE(abs(ga->getCrossRate() - 0.8) <= eps);
        REQUIRE(ga->getMaxEvaluations() == 100);
        REQUIRE(ga->getPopulationSize() == 100);
        REQUIRE(ga->getProblem()->getName() == "Sphere");
    }

    SECTION("Creating a FIGA") {
        auto figa = make_unique<FIGA<FloatSolution>>();
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::FIGA)
                .toSolve(make_unique<Sphere>(100))
                .with()
                .populationOf(100)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(100);
        CHECK(figa);
        CHECK(ga);
        REQUIRE(figa->getName() == ga->getName());
        REQUIRE(ga->getMutation()->getName() == "Uniform All Mutation");
        REQUIRE(ga->getCrossover()->getName() == "Uniform One Crossover");
        REQUIRE(ga->getSelection()->getName() == "Binary Tournament Selection");
        REQUIRE(abs(ga->getMutationRate() - 0.05) <= eps);
        REQUIRE(abs(ga->getCrossRate() - 0.8) <= eps);
        REQUIRE(ga->getMaxEvaluations() == 100);
        REQUIRE(ga->getPopulationSize() == 100);
        REQUIRE(ga->getProblem()->getName() == "Sphere");
    }

    SECTION("Creating a ParallelGA") {
        auto parGA = make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        unique_ptr<ParallelGeneticAlgorithm<FloatSolution>> ga =
            ParGABuilder<FloatSolution>::create()
                .usingCores(10)
                .toSolve(make_unique<Sphere>(100))
                .with()
                .populationOf(100)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.01)
                .withCrossRate(0.6)
                .runDuring(100);
        CHECK(parGA);
        CHECK(ga);
        REQUIRE(parGA->getName() == ga->getName());
        REQUIRE(ga->getMutation()->getName() == "Uniform All Mutation");
        REQUIRE(ga->getCrossover()->getName() == "Uniform One Crossover");
        REQUIRE(ga->getSelection()->getName() == "Binary Tournament Selection");
        REQUIRE(abs(ga->getMutationRate() - 0.01) <= eps);
        REQUIRE(abs(ga->getCrossRate() - 0.6) <= eps);
        REQUIRE(ga->getMaxEvaluations() == 100);
        REQUIRE(ga->getPopulationSize() == 100);
        REQUIRE(ga->getProblem()->getName() == "Sphere");
    }
    std::cout.clear();
    std::cerr.clear();
}