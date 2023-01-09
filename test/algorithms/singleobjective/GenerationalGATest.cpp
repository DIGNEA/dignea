//
// Created by amarrero on 21/12/20.
//

#include <dignea/algorithms/singleobjective/GenerationalGA.h>
#include <dignea/builders/GABuilder.h>
#include <dignea/core/Front.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/problems/SampleProblem.h>
#include <dignea/problems/Sphere.h>
#include <dignea/selections/BinaryTournamentSelection.h>

#include <memory>
#include <typeinfo>
#include <vector>

#include <catch2/catch_all.hpp>

using namespace std;

TEST_CASE("EGenerational Genetic Algorithm Tests", "[GenerationalGA]") {
    int dimension = 10;
    int evals = 1000;
    int populationSize = 32;

    SECTION("Default creation of GenerationalGA") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<GenerationalGA<FloatSolution>>();
        unique_ptr<Mutation<FloatSolution>> mutation =
            make_unique<UniformOneMutation<FloatSolution>>();
        unique_ptr<Crossover<FloatSolution>> crossover =
            make_unique<UniformCrossover<FloatSolution>>();
        unique_ptr<Selection<FloatSolution>> selection =
            make_unique<BinaryTournamentSelection<FloatSolution>>();
        unique_ptr<PopulationEvaluator<FloatSolution>> evaluator =
            make_unique<PopulationEvaluator<FloatSolution>>();
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

    SECTION("Getting GGA-ID") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<GenerationalGA<FloatSolution>>();
        REQUIRE(ga->getID() == "GGA");
    }

    SECTION("Setting maxEvaluations") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<GenerationalGA<FloatSolution>>();
        ga->setMaxEvaluations(evals);
        REQUIRE(ga->getMaxEvaluations() == evals);
    }

    SECTION("Setting populationSize") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<GenerationalGA<FloatSolution>>();
        ga->setPopulationSize(populationSize);
        REQUIRE(ga->getPopulationSize() == populationSize);
    }

    SECTION("Setting a Multi-objective problem") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<GenerationalGA<FloatSolution>>();
        auto sample = make_unique<SampleProblem>(dimension);
        REQUIRE_THROWS(ga->setProblem(move(sample)));
    }

    SECTION("Creating a EGenerational GA with a Multi-objective problem") {
        auto sample = make_unique<Sphere>(100);
        sample->setNumberOfObjs(2);
        auto ga = make_unique<GenerationalGA<FloatSolution>>();
        REQUIRE_THROWS(ga->setProblem(move(sample)));
    }

    SECTION("Creating GenerationalGA with Problem and evals") {
        auto sphere = make_unique<Sphere>(dimension);
        auto ga = make_unique<GenerationalGA<FloatSolution>>();
        ga->setProblem(move(sphere));
        ga->setMaxEvaluations(evals);
        REQUIRE(ga->getMutationRate() == ga->DEFAULT_MUTATION_RATE);
        REQUIRE(ga->getCrossRate() == ga->DEFAULT_CROSSOVER_RATE);
        REQUIRE(ga->getPopulationSize() == ga->DEFAULT_POPULATION_SIZE);
        REQUIRE(ga->getMaxEvaluations() == evals);
        REQUIRE(ga->getProblem()->getName() == "Sphere");
        REQUIRE(ga->getMutation() == nullptr);
        REQUIRE(ga->getCrossover() == nullptr);
        REQUIRE(ga->getSelection() == nullptr);
    }

    SECTION("GenerationalGA runs perfectly") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::Generational)
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
    }
}