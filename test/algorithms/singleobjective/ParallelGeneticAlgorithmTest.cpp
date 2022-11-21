//
// Created by amarrero on 1/2/21.
//

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/problems/SampleProblem.h>
#include <dignea/problems/Sphere.h>
#include <dignea/selections/BinaryTournamentSelection.h>

#include <nlohmann/json.hpp>
#include <vector>

#include "catch2/catch.hpp"

using namespace std;
using json = nlohmann::json;

TEST_CASE("Parallel Genetic Algorithm Tests", "[ParallelGeneticAlgorithm]") {
    int dimension = 10;
    int evals = 1000;
    int populationSize = 32;
    float mutationRate = 0.02;
    float crossoverRate = 0.7;

    SECTION("Default creation of ParallelGeneticAlgorithm") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        REQUIRE_FALSE(ga == nullptr);
        REQUIRE(ga->getMutationRate() == ga->DEFAULT_MUTATION_RATE);
        REQUIRE(ga->getCrossRate() == ga->DEFAULT_CROSSOVER_RATE);
        REQUIRE(ga->getPopulationSize() == ga->DEFAULT_POPULATION_SIZE);
        REQUIRE(ga->getMaxEvaluations() == ga->DEFAULT_EVALUATIONS_LIMIT);
        REQUIRE(ga->getProblem() == nullptr);
        REQUIRE(ga->getMutation() == nullptr);
        REQUIRE(ga->getCrossover() == nullptr);
        REQUIRE(ga->getSelection() == nullptr);
    }

    SECTION("Setting cores") {
        std::array<int, 10> cores = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        unique_ptr<ParallelGeneticAlgorithm<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        for (int nCores : cores) {
            ga->setNumberOfCores(nCores);
            REQUIRE(ga->getNumberOfCores() == nCores);
        }
    }

    SECTION("Getting PGA-ID") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        REQUIRE(ga->getID() == "ParGA");
    }

    SECTION("Setting maxEvaluations") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        ga->setMaxEvaluations(evals);
        REQUIRE(ga->getMaxEvaluations() == evals);
    }

    SECTION("Setting populationSize") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        ga->setPopulationSize(populationSize);
        REQUIRE(ga->getPopulationSize() == populationSize);
    }

    SECTION("Setting a Multi-objective problem") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        unique_ptr<Problem<FloatSolution>> sample =
            make_unique<SampleProblem>(dimension);
        REQUIRE_THROWS(ga->setProblem(move(sample)));
    }

    SECTION("Creating a Parallel GA with a Multi-objective problem") {
        unique_ptr<Problem<FloatSolution>> sample =
            make_unique<SampleProblem>(dimension);
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        REQUIRE_THROWS(ga->setProblem(move(sample)));
    }

    SECTION("Creating ParallelGeneticAlgorithm with Problem and evals") {
        unique_ptr<Problem<FloatSolution>> sphere =
            make_unique<Sphere>(dimension);
        unique_ptr<ParallelGeneticAlgorithm<FloatSolution>> ga =
            make_unique<ParallelGeneticAlgorithm<FloatSolution>>();
        ga->setProblem(move(sphere));
        ga->setMaxEvaluations(evals);
        REQUIRE(ga->getMutationRate() == 0.05);
        REQUIRE(ga->getCrossRate() == 0.8);
        REQUIRE(ga->getPopulationSize() == 32);
        REQUIRE(ga->getMaxEvaluations() == evals);
        REQUIRE(ga->getProblem()->getName() == "Sphere");
        REQUIRE(ga->getNumberOfCores() == 2);
        REQUIRE(ga->getMutation() == nullptr);
        REQUIRE(ga->getCrossover() == nullptr);
        REQUIRE(ga->getSelection() == nullptr);
    }

    SECTION("ParallelGeneticAlgorithm runs perfectly") {
        unique_ptr<ParallelGeneticAlgorithm<FloatSolution>> ga =
            ParGABuilder<FloatSolution>::create()
                .usingCores(2)
                .toSolve(make_unique<Sphere>(dimension))
                .with()
                .populationOf(populationSize)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(evals);
        ga->run();
        REQUIRE(ga->getPerformedEvaluations() >= ga->getMaxEvaluations());
        REQUIRE_FALSE(ga->getElapsedTime() == 0.0);
        Front<FloatSolution> solutions = ga->getResults();
        REQUIRE_FALSE(solutions.getNumOfSolutions() == 0);
        for (FloatSolution &solution : solutions.getSolutions()) {
            REQUIRE_FALSE((&solution) == nullptr);
            REQUIRE(solution.getNumberOfVars() == dimension);
            REQUIRE(solution.getNumberOfObjs() == 1);
            REQUIRE(solution.getNumberOfCons() == 0);
        }
    }

    SECTION("ParallelGeneticAlgorithm to json") {
        unique_ptr<ParallelGeneticAlgorithm<FloatSolution>> ga =
            ParGABuilder<FloatSolution>::create()
                .usingCores(4)
                .toSolve(make_unique<Sphere>(dimension))
                .with()
                .populationOf(populationSize)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(mutationRate)
                .withCrossRate(crossoverRate)
                .runDuring(evals);

        json data = ga->to_json();
        float mRateFromData = data["mutation_rate"];
        REQUIRE(mRateFromData == mutationRate);
        REQUIRE(data["crossover_rate"] == crossoverRate);
        REQUIRE(data["num_cores"] == 4);
        REQUIRE(data["name"] == ga->getName());
        REQUIRE(data["max_evals"] == ga->getMaxEvaluations());
        REQUIRE(data["pop_size"] == ga->getPopulationSize());
        REQUIRE(data["elapsed_time"] == ga->getElapsedTime());
        REQUIRE(data["evaluator"] == ga->getEvaluator()->getName());
    }
}