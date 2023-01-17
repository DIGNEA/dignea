//
// Created by amarrero on 15/4/21.
//

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/generator/EIG.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/mutations/UniformAllMutation.h>
#include <dignea/problems/KPNR.h>
#include <dignea/types/SolutionTypes.h>

#include <chrono>
#include <iostream>  // std::cout
#include <nlohmann/json.hpp>
#include <typeinfo>  // operator typeid
#include <vector>

#include <catch2/catch_all.hpp>
using json = nlohmann::json;
using namespace std;

TEST_CASE("EIG tests", "[EIG]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);

    using IGP = KPDomain;
    using IS = KPInstance;
    using OP = KPNR;
    using OS = BoolFloatSolution;
    using AEA = AbstractSolver<OS>;

    SECTION("Creating a EIG object") {
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        REQUIRE(generator);
        REQUIRE(generator->getEvaluation() != nullptr);
    }

    SECTION("EIG getName") {
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        REQUIRE(generator != nullptr);
        REQUIRE(generator->getName() == "EIG");
    }

    SECTION("EIG getID") {
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        REQUIRE(generator != nullptr);
        REQUIRE(generator->getID() == "EIG");
    }

    SECTION("Creating a EIG object with configurations") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<ParallelGeneticAlgorithm<OS>>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        auto mutation = make_unique<UniformOneMutation<IS>>();
        auto crossover = make_unique<UniformCrossover<IS>>();
        auto selection = make_unique<BinaryTournamentSelection<IS>>();

        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        generator->setPortfolio(configurations);
        generator->setInstanceFitness(move(evaluation));
        generator->setMaxEvaluations(generations);
        generator->setPopulationSize(popSize);
        generator->setMutationRate(mutRate);
        generator->setCrossoverRate(crossRate);
        generator->setRepetitions(reps);
        generator->setMutation(move(mutation));
        generator->setCrossover(move(crossover));
        generator->setSelection(move(selection));
        generator->setInstanceProblem(move(problem));
        REQUIRE(generator);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == popSize);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == mutRate);
        REQUIRE(generator->getCrossoverRate() == crossRate);
        REQUIRE(generator->getPortfolio().size() == 1);
        REQUIRE(generator->getInstanceProblem()->getName() == problemCopy->getName());

        auto mutCopy = make_unique<UniformOneMutation<IS>>();
        auto CXCopy = make_unique<UniformCrossover<IS>>();
        auto selCopy = make_unique<BinaryTournamentSelection<IS>>();
        REQUIRE(generator->getMutation()->getName() == mutCopy->getName());
        REQUIRE(generator->getCrossover()->getName() == CXCopy->getName());
        REQUIRE(generator->getSelection()->getName() == selCopy->getName());
    }

    SECTION("Creating a EIG object with configurations and operators") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<ParallelGeneticAlgorithm<OS>>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        auto evaluation = make_unique<EasyInstances>();
        auto mutation = make_unique<UniformOneMutation<IS>>();
        auto crossover = make_unique<UniformCrossover<IS>>();
        auto selection = make_unique<BinaryTournamentSelection<IS>>();
        auto mutCopy = make_unique<UniformOneMutation<IS>>();
        auto crossCopy = make_unique<UniformCrossover<IS>>();
        auto selectionCopy = make_unique<BinaryTournamentSelection<IS>>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        generator->setPortfolio(configurations);
        generator->setInstanceFitness(move(evaluation));
        generator->setMaxEvaluations(generations);
        generator->setPopulationSize(popSize);
        generator->setMutationRate(mutRate);
        generator->setCrossoverRate(crossRate);
        generator->setRepetitions(reps);
        generator->setMutation(move(mutation));
        generator->setCrossover(move(crossover));
        generator->setSelection(move(selection));
        generator->setInstanceProblem(move(problem));
        REQUIRE(generator);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == popSize);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == mutRate);
        REQUIRE(generator->getCrossoverRate() == crossRate);
        REQUIRE(generator->getPortfolio().size() == 1);
        REQUIRE(generator->getInstanceProblem()->getName() == problemCopy->getName());

        REQUIRE(generator->getMutation()->getName() == mutCopy->getName());
        REQUIRE(generator->getCrossover()->getName() == crossCopy->getName());
        REQUIRE(generator->getSelection()->getName() == selectionCopy->getName());
    }

    SECTION("Setting CrossoverRate in EIG") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<ParallelGeneticAlgorithm<OS>>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        auto mutation = make_unique<UniformOneMutation<IS>>();
        auto crossover = make_unique<UniformCrossover<IS>>();
        auto selection = make_unique<BinaryTournamentSelection<IS>>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        generator->setPortfolio(configurations);
        generator->setInstanceFitness(move(evaluation));
        generator->setMaxEvaluations(generations);
        generator->setPopulationSize(popSize);
        generator->setMutationRate(mutRate);
        generator->setCrossoverRate(crossRate);
        generator->setRepetitions(reps);
        generator->setMutation(move(mutation));
        generator->setCrossover(move(crossover));
        generator->setSelection(move(selection));
        generator->setInstanceProblem(move(problem));
        REQUIRE(generator);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == popSize);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == mutRate);
        REQUIRE(generator->getCrossoverRate() == crossRate);
        REQUIRE(generator->getPortfolio().size() == 1);
        REQUIRE(generator->getInstanceProblem()->getName() == problemCopy->getName());

        auto mutCopy = make_unique<UniformOneMutation<IS>>();
        auto CXCopy = make_unique<UniformCrossover<IS>>();
        auto selCopy = make_unique<BinaryTournamentSelection<IS>>();
        REQUIRE(generator->getMutation()->getName() == mutCopy->getName());
        REQUIRE(generator->getCrossover()->getName() == CXCopy->getName());
        REQUIRE(generator->getSelection()->getName() == selCopy->getName());
        float newCrossRate = 0.8;
        generator->setCrossoverRate(newCrossRate);
        REQUIRE(generator->getCrossoverRate() == newCrossRate);
    }

    SECTION("Setting MutationRate in EIG") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<ParallelGeneticAlgorithm<OS>>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        generator->setPortfolio(configurations);
        generator->setInstanceFitness(move(evaluation));
        generator->setMaxEvaluations(generations);
        generator->setPopulationSize(popSize);
        generator->setMutationRate(mutRate);
        generator->setCrossoverRate(crossRate);
        generator->setRepetitions(reps);
        generator->setInstanceProblem(move(problem));
        REQUIRE(generator);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == popSize);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == mutRate);
        REQUIRE(generator->getCrossoverRate() == crossRate);
        REQUIRE(generator->getPortfolio().size() == 1);
        REQUIRE(generator->getInstanceProblem()->getName() == problemCopy->getName());

        float newMutaRate = 0.1;
        generator->setMutationRate(newMutaRate);
        REQUIRE(generator->getMutationRate() == newMutaRate);
    }

    SECTION("Setting repetitions in EIG") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<ParallelGeneticAlgorithm<OS>>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        generator->setPortfolio(configurations);
        generator->setInstanceFitness(move(evaluation));
        generator->setMaxEvaluations(generations);
        generator->setPopulationSize(popSize);
        generator->setMutationRate(mutRate);
        generator->setCrossoverRate(crossRate);
        generator->setRepetitions(reps);
        generator->setInstanceProblem(move(problem));
        REQUIRE(generator);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == popSize);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == mutRate);
        REQUIRE(generator->getCrossoverRate() == crossRate);
        REQUIRE(generator->getPortfolio().size() == 1);
        REQUIRE(generator->getInstanceProblem()->getName() == problemCopy->getName());

        int newReps = 1000;
        generator->setRepetitions(newReps);
        REQUIRE(generator->getRepetitions() == newReps);
    }

    SECTION("Setting Mutation in EIG") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<ParallelGeneticAlgorithm<OS>>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        generator->setPortfolio(configurations);
        generator->setInstanceFitness(move(evaluation));
        generator->setMaxEvaluations(generations);
        generator->setPopulationSize(popSize);
        generator->setMutationRate(mutRate);
        generator->setCrossoverRate(crossRate);
        generator->setRepetitions(reps);
        generator->setInstanceProblem(move(problem));
        REQUIRE(generator);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == popSize);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == mutRate);
        REQUIRE(generator->getCrossoverRate() == crossRate);
        REQUIRE(generator->getPortfolio().size() == 1);
        REQUIRE(generator->getInstanceProblem()->getName() == problemCopy->getName());

        unique_ptr<Mutation<IS>> mutation =
            make_unique<UniformAllMutation<IS>>();
        unique_ptr<Mutation<IS>> mutCopy =
            make_unique<UniformAllMutation<IS>>();
        generator->setMutation(move(mutation));
        REQUIRE(generator->getMutation());
    }

    SECTION("Setting Crossover in EIG") {
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        unique_ptr<Crossover<IS>> crossover =
            make_unique<UniformCrossover<IS>>();
        unique_ptr<Crossover<IS>> crossoverCopy =
            make_unique<UniformCrossover<IS>>();
        generator->setCrossover(move(crossover));
        REQUIRE(generator->getCrossover());
    }

    SECTION("Setting Selection in EIG") {
        auto generator = make_unique<EIG<IGP, IS, OP, OS>>();
        unique_ptr<Selection<IS>> selection =
            make_unique<BinaryTournamentSelection<IS>>();
        unique_ptr<Selection<IS>> selectionCopy =
            make_unique<BinaryTournamentSelection<IS>>();
        generator->setSelection(move(selection));
        REQUIRE(generator->getSelection());
    }

    SECTION("EIG can generate KP instances") {
        using OS = BoolFloatSolution;
        using OP = KPNR;
        using IS = KPInstance;
        using IP = KPDomain;
        using EA = AbstractSolver<OS>;

        // EIG Parameters
        auto generations = 2;
        auto reps = 1;
        auto meaCXRate = 0.8;
        auto nInstances = 10;
        auto easyEvaluator = make_unique<EasyInstances>();

        // KP instances parameters
        auto upperBound = 1000;
        auto instanceSize = 100;
        // EAs configurations parameters
        auto maxEvals = 100;
        auto crossRates = vector<float>{0.7, 0.8};
        float fitnessRatio = 0.7;
        float diversityRatio = 0.3;
        auto mutationRate = 1.0 / float(instanceSize);
        // Novelty Search parameters
        auto thresholdNS = 3;
        auto k = 3;
        auto distance = make_unique<Euclidean<float>>();
        auto novelty = NSFactory<IS>().create(
            NSType::Features, make_unique<Euclidean<float>>(), thresholdNS, k);
        json novelty_json = novelty->to_json();

        vector<unique_ptr<EA>> algorithms;
        auto instKP = make_unique<KPDomain>(instanceSize, nInstances, 1,
                                              upperBound, 1, upperBound);

        // Building the EA configurations
        for (float c : crossRates) {
            unique_ptr<ParallelGeneticAlgorithm<OS>> algorithm =
                ParGABuilder<OS>::create()
                    .usingCores(32)
                    .with()
                    .mutation(MutType::UniformOne)
                    .crossover(CXType::Uniform)
                    .selection(SelType::Binary)
                    .populationOf(32)
                    .withMutRate(mutationRate)
                    .withCrossRate(c)
                    .runDuring(maxEvals);
            algorithms.push_back(move(algorithm));
        }

        // Building the EIG
        unique_ptr<EIG<IP, IS, OP, OS>> generator =
            EIGBuilder<IP, IS, OP, OS>::create()
                .toSolve(move(instKP))
                .with()
                .weights(fitnessRatio, diversityRatio)
                .portfolio(algorithms)
                .evalWith(move(easyEvaluator))
                .repeating(reps)
                .withSearch(NSType::Features, move(distance), thresholdNS, k)
                .with()
                .crossover(CXType::Uniform)
                .mutation(MutType::UniformOne)
                .selection(SelType::Binary)
                .withMutRate(mutationRate)
                .withCrossRate(meaCXRate)
                .populationOf(nInstances)
                .runDuring(generations);

        REQUIRE_FALSE(generator == nullptr);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == nInstances);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == (float)mutationRate);
        REQUIRE(generator->getCrossoverRate() == (float)meaCXRate);
        REQUIRE(generator->getPortfolio().size() == 2);
        REQUIRE(generator->getInstanceProblem()->getName() == "KPDomain");
        // Heavy stuff
        REQUIRE_NOTHROW(generator->run());
        REQUIRE(generator->getElapsedTime() != 0.0);
        auto front = generator->getResults();
        REQUIRE(!front.empty());

        // Reused the work to tests the JSON

        SECTION("Test EIG to_json after run") {
            json data = generator->to_json();
            REQUIRE(data["name"] == generator->getName());
            REQUIRE(data["max_generations"] == generator->getMaxEvaluations());
            REQUIRE(data["repetitions"] == generator->getRepetitions());
            REQUIRE(data["pop_size"] == generator->getPopulationSize());
            REQUIRE(data["mutation"] == generator->getMutation()->getName());
            REQUIRE(data["crossover"] == generator->getCrossover()->getName());
            REQUIRE(data["mutation_rate"] == generator->getMutationRate());
            REQUIRE(data["crossover_rate"] == generator->getCrossoverRate());
            REQUIRE(data["elapsed_time"] == generator->getElapsedTime());
            REQUIRE(data["evolution"].is_array());
            REQUIRE(data["avg_evolution"].is_array());
            REQUIRE(data["novelty_search"].is_object());

            REQUIRE("NSFeatures" == novelty_json["name"]);
            REQUIRE(novelty_json["threshold"] == 3);
            REQUIRE(novelty_json["k"] == 3);
            REQUIRE(data["weighted_function"]["fitness_ratio"] == fitnessRatio);
            REQUIRE(data["weighted_function"]["novelty_ratio"] ==
                    diversityRatio);
        }
    }
}