//
// Created by amarrero on 15/4/21.
//

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/MEABuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/mea/MEA.h>
#include <dignea/mea/problems/IKPProblem.h>
#include <dignea/mea/solutions/IKPSolution.h>
#include <dignea/mutations/UniformAllMutation.h>
#include <dignea/problems/KPNR.h>
#include <dignea/types/SolutionTypes.h>

#include <chrono>
#include <iostream>  // std::cout
#include <nlohmann/json.hpp>
#include <typeinfo>  // operator typeid
#include <vector>

#include "catch2/catch.hpp"
using json = nlohmann::json;
using namespace std;

TEST_CASE("MEA tests", "[MEA]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);

    using IGP = IKPProblem;
    using IS = IKPSolution;
    using OP = KPNR;
    using OS = BoolFloatSolution;
    using AEA = AbstractEA<OS>;

    SECTION("Creating a MEA object") {
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        REQUIRE(mea);
        REQUIRE(mea->getEvaluation() != nullptr);
    }

    SECTION("MEA getName") {
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        REQUIRE(mea != nullptr);
        REQUIRE(mea->getName() == "MEA");
    }

    SECTION("MEA getID") {
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        REQUIRE(mea != nullptr);
        REQUIRE(mea->getID() == "MEA");
    }

    SECTION("Creating a MEA object with configurations") {
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
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        mea->setPortfolio(configurations);
        mea->setInstanceFitness(move(evaluation));
        mea->setMaxEvaluations(generations);
        mea->setPopulationSize(popSize);
        mea->setMutationRate(mutRate);
        mea->setCrossoverRate(crossRate);
        mea->setRepetitions(reps);
        mea->setMutation(move(mutation));
        mea->setCrossover(move(crossover));
        mea->setSelection(move(selection));
        mea->setInstanceProblem(move(problem));
        REQUIRE(mea);
        REQUIRE(mea->getGenerations() == generations);
        REQUIRE(mea->getPopulationSize() == popSize);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getMutationRate() == mutRate);
        REQUIRE(mea->getCrossoverRate() == crossRate);
        REQUIRE(mea->getPortfolio().size() == 1);
        REQUIRE(mea->getInstanceProblem()->getName() == problemCopy->getName());

        auto mutCopy = make_unique<UniformOneMutation<IS>>();
        auto CXCopy = make_unique<UniformCrossover<IS>>();
        auto selCopy = make_unique<BinaryTournamentSelection<IS>>();
        REQUIRE(mea->getMutation()->getName() == mutCopy->getName());
        REQUIRE(mea->getCrossover()->getName() == CXCopy->getName());
        REQUIRE(mea->getSelection()->getName() == selCopy->getName());
    }

    SECTION("Creating a MEA object with configurations and operators") {
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
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        mea->setPortfolio(configurations);
        mea->setInstanceFitness(move(evaluation));
        mea->setMaxEvaluations(generations);
        mea->setPopulationSize(popSize);
        mea->setMutationRate(mutRate);
        mea->setCrossoverRate(crossRate);
        mea->setRepetitions(reps);
        mea->setMutation(move(mutation));
        mea->setCrossover(move(crossover));
        mea->setSelection(move(selection));
        mea->setInstanceProblem(move(problem));
        REQUIRE(mea);
        REQUIRE(mea->getGenerations() == generations);
        REQUIRE(mea->getPopulationSize() == popSize);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getMutationRate() == mutRate);
        REQUIRE(mea->getCrossoverRate() == crossRate);
        REQUIRE(mea->getPortfolio().size() == 1);
        REQUIRE(mea->getInstanceProblem()->getName() == problemCopy->getName());

        REQUIRE(mea->getMutation()->getName() == mutCopy->getName());
        REQUIRE(mea->getCrossover()->getName() == crossCopy->getName());
        REQUIRE(mea->getSelection()->getName() == selectionCopy->getName());
    }

    SECTION("Setting CrossoverRate in MEA") {
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
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        mea->setPortfolio(configurations);
        mea->setInstanceFitness(move(evaluation));
        mea->setMaxEvaluations(generations);
        mea->setPopulationSize(popSize);
        mea->setMutationRate(mutRate);
        mea->setCrossoverRate(crossRate);
        mea->setRepetitions(reps);
        mea->setMutation(move(mutation));
        mea->setCrossover(move(crossover));
        mea->setSelection(move(selection));
        mea->setInstanceProblem(move(problem));
        REQUIRE(mea);
        REQUIRE(mea->getGenerations() == generations);
        REQUIRE(mea->getPopulationSize() == popSize);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getMutationRate() == mutRate);
        REQUIRE(mea->getCrossoverRate() == crossRate);
        REQUIRE(mea->getPortfolio().size() == 1);
        REQUIRE(mea->getInstanceProblem()->getName() == problemCopy->getName());

        auto mutCopy = make_unique<UniformOneMutation<IS>>();
        auto CXCopy = make_unique<UniformCrossover<IS>>();
        auto selCopy = make_unique<BinaryTournamentSelection<IS>>();
        REQUIRE(mea->getMutation()->getName() == mutCopy->getName());
        REQUIRE(mea->getCrossover()->getName() == CXCopy->getName());
        REQUIRE(mea->getSelection()->getName() == selCopy->getName());
        float newCrossRate = 0.8;
        mea->setCrossoverRate(newCrossRate);
        REQUIRE(mea->getCrossoverRate() == newCrossRate);
    }

    SECTION("Setting MutationRate in MEA") {
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
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        mea->setPortfolio(configurations);
        mea->setInstanceFitness(move(evaluation));
        mea->setMaxEvaluations(generations);
        mea->setPopulationSize(popSize);
        mea->setMutationRate(mutRate);
        mea->setCrossoverRate(crossRate);
        mea->setRepetitions(reps);
        mea->setInstanceProblem(move(problem));
        REQUIRE(mea);
        REQUIRE(mea->getGenerations() == generations);
        REQUIRE(mea->getPopulationSize() == popSize);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getMutationRate() == mutRate);
        REQUIRE(mea->getCrossoverRate() == crossRate);
        REQUIRE(mea->getPortfolio().size() == 1);
        REQUIRE(mea->getInstanceProblem()->getName() == problemCopy->getName());

        float newMutaRate = 0.1;
        mea->setMutationRate(newMutaRate);
        REQUIRE(mea->getMutationRate() == newMutaRate);
    }

    SECTION("Setting repetitions in MEA") {
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
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        mea->setPortfolio(configurations);
        mea->setInstanceFitness(move(evaluation));
        mea->setMaxEvaluations(generations);
        mea->setPopulationSize(popSize);
        mea->setMutationRate(mutRate);
        mea->setCrossoverRate(crossRate);
        mea->setRepetitions(reps);
        mea->setInstanceProblem(move(problem));
        REQUIRE(mea);
        REQUIRE(mea->getGenerations() == generations);
        REQUIRE(mea->getPopulationSize() == popSize);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getMutationRate() == mutRate);
        REQUIRE(mea->getCrossoverRate() == crossRate);
        REQUIRE(mea->getPortfolio().size() == 1);
        REQUIRE(mea->getInstanceProblem()->getName() == problemCopy->getName());

        int newReps = 1000;
        mea->setRepetitions(newReps);
        REQUIRE(mea->getRepetitions() == newReps);
    }

    SECTION("Setting Mutation in MEA") {
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
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        mea->setPortfolio(configurations);
        mea->setInstanceFitness(move(evaluation));
        mea->setMaxEvaluations(generations);
        mea->setPopulationSize(popSize);
        mea->setMutationRate(mutRate);
        mea->setCrossoverRate(crossRate);
        mea->setRepetitions(reps);
        mea->setInstanceProblem(move(problem));
        REQUIRE(mea);
        REQUIRE(mea->getGenerations() == generations);
        REQUIRE(mea->getPopulationSize() == popSize);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getMutationRate() == mutRate);
        REQUIRE(mea->getCrossoverRate() == crossRate);
        REQUIRE(mea->getPortfolio().size() == 1);
        REQUIRE(mea->getInstanceProblem()->getName() == problemCopy->getName());

        unique_ptr<Mutation<IS>> mutation =
            make_unique<UniformAllMutation<IS>>();
        unique_ptr<Mutation<IS>> mutCopy =
            make_unique<UniformAllMutation<IS>>();
        mea->setMutation(move(mutation));
        REQUIRE(mea->getMutation());
    }

    SECTION("Setting Crossover in MEA") {
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        unique_ptr<Crossover<IS>> crossover =
            make_unique<UniformCrossover<IS>>();
        unique_ptr<Crossover<IS>> crossoverCopy =
            make_unique<UniformCrossover<IS>>();
        mea->setCrossover(move(crossover));
        REQUIRE(mea->getCrossover());
    }

    SECTION("Setting Selection in MEA") {
        auto mea = make_unique<MEA<IGP, IS, OP, OS>>();
        unique_ptr<Selection<IS>> selection =
            make_unique<BinaryTournamentSelection<IS>>();
        unique_ptr<Selection<IS>> selectionCopy =
            make_unique<BinaryTournamentSelection<IS>>();
        mea->setSelection(move(selection));
        REQUIRE(mea->getSelection());
    }

    SECTION("MEA can generate KP instances") {
        using OS = BoolFloatSolution;
        using OP = KPNR;
        using IS = IKPSolution;
        using IP = IKPProblem;
        using EA = AbstractEA<OS>;

        // MEA Parameters
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
        auto instKP = make_unique<IKPProblem>(instanceSize, nInstances, 1,
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

        // Building the MEA
        unique_ptr<MEA<IP, IS, OP, OS>> mea =
            MEABuilder<IP, IS, OP, OS>::create()
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

        REQUIRE_FALSE(mea == nullptr);
        REQUIRE(mea->getGenerations() == generations);
        REQUIRE(mea->getPopulationSize() == nInstances);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getMutationRate() == (float)mutationRate);
        REQUIRE(mea->getCrossoverRate() == (float)meaCXRate);
        REQUIRE(mea->getPortfolio().size() == 2);
        REQUIRE(mea->getInstanceProblem()->getName() == "IKPProblem");
        // Heavy stuff
        REQUIRE_NOTHROW(mea->run());
        REQUIRE(mea->getElapsedTime() != 0.0);
        auto front = mea->getResults();
        REQUIRE(!front.empty());

        // Reused the work to tests the JSON

        SECTION("Test MEA to_json after run") {
            json data = mea->to_json();
            REQUIRE(data["name"] == mea->getName());
            REQUIRE(data["max_generations"] == mea->getMaxEvaluations());
            REQUIRE(data["repetitions"] == mea->getRepetitions());
            REQUIRE(data["pop_size"] == mea->getPopulationSize());
            REQUIRE(data["mutation"] == mea->getMutation()->getName());
            REQUIRE(data["crossover"] == mea->getCrossover()->getName());
            REQUIRE(data["mutation_rate"] == mea->getMutationRate());
            REQUIRE(data["crossover_rate"] == mea->getCrossoverRate());
            REQUIRE(data["elapsed_time"] == mea->getElapsedTime());
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