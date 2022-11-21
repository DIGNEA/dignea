//
// Created by amarrero on 21/12/20.
//
#include <dignea/algorithms/singleobjective/SteadyGA.h>
#include <dignea/builders/GABuilder.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/problems/SampleProblem.h>
#include <dignea/problems/Sphere.h>
#include <dignea/selections/BinaryTournamentSelection.h>

#include <memory>
#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Steady Genetic Algorithm Tests", "[SteadyGA]") {
    using ProblemType = Problem<FloatSolution>;

    int dimension = 10;
    int evals = 1000;
    int populationSize = 32;
    double mutationRate = 0.02;
    double crossoverRate = 0.7;

    SECTION("Default creation of GenerationalGA") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
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
    SECTION("Getting SGA ID") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
        REQUIRE(ga->getID() == "SSGA");
    }
    SECTION("Setting maxEvaluations") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
        ga->setMaxEvaluations(evals);
        REQUIRE(ga->getMaxEvaluations() == evals);
    }

    SECTION("Setting populationSize") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
        ga->setPopulationSize(populationSize);
        REQUIRE(ga->getPopulationSize() == populationSize);
    }

    SECTION("Setting problem") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
        auto sample = make_unique<Sphere>(dimension);
        auto copy = make_unique<Sphere>(dimension);
        ga->setProblem(move(sample));
        REQUIRE(ga->getProblem()->getName() == copy->getName());
    }

    SECTION("Setting a Multi-objective problem") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
        auto sample = make_unique<SampleProblem>(dimension);
        REQUIRE_THROWS(ga->setProblem(move(sample)));
    }

    SECTION("Creating a SteadyGa with a Multi-objective problem") {
        auto sample = make_unique<SampleProblem>(dimension);
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
        REQUIRE_THROWS(ga->setProblem(move(sample)));
    }

    SECTION("Creating SteadyGA with Problem and evals") {
        unique_ptr<ProblemType> sphere = make_unique<Sphere>(dimension);
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<SteadyGA<FloatSolution>>();
        ga->setMaxEvaluations(evals);
        ga->setProblem(move(sphere));
        REQUIRE(ga->getMutationRate() == ga->DEFAULT_MUTATION_RATE);
        REQUIRE(ga->getCrossRate() == ga->DEFAULT_CROSSOVER_RATE);
        REQUIRE(ga->getPopulationSize() == ga->DEFAULT_POPULATION_SIZE);
        REQUIRE(ga->getMaxEvaluations() == evals);
        REQUIRE(ga->getProblem()->getName() == "Sphere");
        REQUIRE(ga->getMutation() == nullptr);
        REQUIRE(ga->getCrossover() == nullptr);
        REQUIRE(ga->getSelection() == nullptr);
    }

    SECTION("SteadyGA runs perfectly") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::Steady)
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
        ga->run();
        REQUIRE(ga->getPerformedEvaluations() >= ga->getMaxEvaluations());
        REQUIRE_FALSE(ga->getElapsedTime() == 0.0);
        Front<FloatSolution> solutions = ga->getResults();
        REQUIRE_FALSE(solutions.getNumOfSolutions() == 0);
    }
}