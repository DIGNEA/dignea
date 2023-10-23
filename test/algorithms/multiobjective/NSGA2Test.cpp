/**
 * @file NSGA2Test.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-11-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <dignea/algorithms/multiobjective/NSGA2.h>
#include <dignea/builders/GABuilder.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/problems/DoubleSphere.h>
#include <dignea/selections/BinaryTournamentSelection.h>

#include <catch2/catch_all.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

TEST_CASE("Non-Dominated Sorting Genetic Algorith II Tests", "[NSGA-II]") {
    int dimension = 10;
    int evals = 1000;
    int populationSize = 32;

    SECTION("Default creation of NSGA-II") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<NSGA2<FloatSolution>>();
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

    SECTION("Getting NSGA-II ID") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<NSGA2<FloatSolution>>();
        REQUIRE(ga->getID() == "NSGA-II");
    }

    SECTION("Getting NSGA-II name") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<NSGA2<FloatSolution>>();
        REQUIRE(ga->getName() == "Non-dominated Sorting Genetic Algorithm II");
    }

    SECTION("Setting maxEvaluations") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<NSGA2<FloatSolution>>();
        ga->setMaxEvaluations(evals);
        REQUIRE(ga->getMaxEvaluations() == evals);
    }

    SECTION("Setting populationSize") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<NSGA2<FloatSolution>>();
        ga->setPopulationSize(populationSize);
        REQUIRE(ga->getPopulationSize() == populationSize);
    }

    SECTION("Setting a Multi-objective problem") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<NSGA2<FloatSolution>>();
        auto sample = make_unique<DoubleSphere>(dimension);
        REQUIRE_NOTHROW(ga->setProblem(move(sample)));
    }

    SECTION("Creating a SteadyGa with a Multi-objective problem") {
        auto evaluator = make_unique<PopulationEvaluator<FloatSolution>>();
        auto sample = make_unique<DoubleSphere>(dimension);
        unique_ptr<AbstractGA<FloatSolution>> ga =
            make_unique<NSGA2<FloatSolution>>();
        REQUIRE_NOTHROW(ga->setProblem(move(sample)));
    }

    SECTION("Creating NSGA2 with Problem and evals") {
        auto sphere = make_unique<DoubleSphere>(dimension);
        unique_ptr<Mutation<FloatSolution>> mutation =
            make_unique<UniformOneMutation<FloatSolution>>();
        unique_ptr<Crossover<FloatSolution>> crossover =
            make_unique<UniformCrossover<FloatSolution>>();
        unique_ptr<Selection<FloatSolution>> selection =
            make_unique<BinaryTournamentSelection<FloatSolution>>();
        unique_ptr<PopulationEvaluator<FloatSolution>> evaluator =
            make_unique<PopulationEvaluator<FloatSolution>>();

        auto ga = make_unique<NSGA2<FloatSolution>>();
        ga->setProblem(move(sphere));
        ga->setMaxEvaluations(evals);
        REQUIRE(ga->getMutationRate() == ga->DEFAULT_MUTATION_RATE);
        REQUIRE(ga->getCrossRate() == ga->DEFAULT_CROSSOVER_RATE);
        REQUIRE(ga->getPopulationSize() == ga->DEFAULT_POPULATION_SIZE);
        REQUIRE(ga->getMaxEvaluations() == evals);
        REQUIRE(ga->getProblem()->getName() == "DoubleSphere");
        REQUIRE(ga->getMutation() == nullptr);
        REQUIRE(ga->getCrossover() == nullptr);
        REQUIRE(ga->getSelection() == nullptr);
    }

    SECTION("NSGA2 created with Builder") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::NSGA2)
                .toSolve(make_unique<DoubleSphere>(dimension))
                .with()
                .populationOf(populationSize)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(evals);
        REQUIRE(ga);
    }

    SECTION("NSGA2 run to solve Double Sphere") {
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::NSGA2)
                .toSolve(make_unique<DoubleSphere>(dimension))
                .with()
                .populationOf(populationSize)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(evals);
        REQUIRE_NOTHROW(ga->run());
        Front result = ga->getResults();
        REQUIRE_FALSE(result.empty());
        vector bestObjs = result.getSolutions()[0].getObjectives();
        REQUIRE(bestObjs[0] == bestObjs[1]);
    }
}