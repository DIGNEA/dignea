
/**
 * @brief Experiment Builder Tests
 *
 */

#include <dignea/builders/ExpBuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/crossovers/OrderCrossover.h>
#include <dignea/problems/TSP.h>

#include <filesystem>

#include "catch2/catch.hpp"

TEST_CASE("Experiment Builder Tests", "[ExpBuilder]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);

    SECTION("Creating an Experiment") {
        auto popSize = 32;
        auto evaluation = 100;
        auto crossRate = 0.8;
        auto mutationRate = 0.05;
        auto repetitions{10};

        unique_ptr<TSP> tsp = make_unique<TSP>(1000);
        unique_ptr<ParallelGeneticAlgorithm<IntFloatSolution>> ga =
            ParGABuilder<IntFloatSolution>::create()
                .with()
                .populationOf(popSize)
                .with()
                .mutation(MutType::SwapMutation)
                .crossover(CXType::Order)
                .selection(SelType::Binary)
                .withMutRate(mutationRate)
                .withCrossRate(crossRate)
                .runDuring(evaluation)
                .usingCores(popSize);
        unique_ptr<Experiment<IntFloatSolution>> experiment =
            ExpBuilder<IntFloatSolution>::create("exampleExperiment")
                .saveResultsIn("example_test")
                .usingAlgorithm(move(ga))
                .toSolve(move(tsp))
                .repeat(repetitions);

        CHECK(experiment);
        REQUIRE(experiment->getName() == "example_test/exampleExperiment");
        REQUIRE(
            experiment->getAlgorithm()->getName() ==
            "Parallel Genetic Algorithm");  // No env configurated so name is ""
        REQUIRE(experiment->getProblemToSolve()->getName() == "TSP");
        REQUIRE(experiment->getRepetitions() == repetitions);
        // Removing the example_test directory
        std::filesystem::remove("example_test");
    }
}