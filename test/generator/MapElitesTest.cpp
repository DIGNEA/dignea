//
// Created by amarrero on 15/4/21.
//
#include <dignea/algorithms/kp_heuristics/Default.h>
#include <dignea/algorithms/kp_heuristics/MPW.h>
#include <dignea/algorithms/kp_heuristics/MaP.h>
#include <dignea/algorithms/kp_heuristics/MiW.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/generator/MapElites.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/mutations/UniformAllMutation.h>
#include <dignea/problems/KPNR.h>
#include <dignea/types/SolutionTypes.h>

#include <catch2/catch_all.hpp>
#include <chrono>
#include <iostream>  // std::cout
#include <nlohmann/json.hpp>
#include <typeinfo>  // operator typeid
#include <vector>
using json = nlohmann::json;
using namespace std;

TEST_CASE("MapElites tests", "[MapElites]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);

    using IGP = KPDomain;
    using IS = KPInstance;
    using OP = KPNR;
    using OS = BoolFloatSolution;
    using AEA = AbstractEA<OS>;

    SECTION("Creating a MapElites object") {
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
        REQUIRE(generator);
        REQUIRE(generator->getEvaluation() != nullptr);
    }

    SECTION("MapElites getName") {
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
        REQUIRE(generator != nullptr);
        REQUIRE(generator->getName() == "MapElites");
    }

    SECTION("MapElites getID") {
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
        REQUIRE(generator != nullptr);
        REQUIRE(generator->getID() == "MapElites");
    }

    SECTION("Creating a MapElites object with configurations") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<Default>());
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
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
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
        REQUIRE(generator->getInstanceProblem()->getName() ==
                problemCopy->getName());

        auto mutCopy = make_unique<UniformOneMutation<IS>>();
        auto CXCopy = make_unique<UniformCrossover<IS>>();
        auto selCopy = make_unique<BinaryTournamentSelection<IS>>();
        REQUIRE(generator->getMutation()->getName() == mutCopy->getName());
        REQUIRE(generator->getCrossover()->getName() == CXCopy->getName());
        REQUIRE(generator->getSelection()->getName() == selCopy->getName());
    }

    SECTION("Creating a MapElites object with configurations and operators") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<Default>());
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
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
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
        REQUIRE(generator->getInstanceProblem()->getName() ==
                problemCopy->getName());

        REQUIRE(generator->getMutation()->getName() == mutCopy->getName());
        REQUIRE(generator->getCrossover()->getName() == crossCopy->getName());
        REQUIRE(generator->getSelection()->getName() ==
                selectionCopy->getName());
    }

    SECTION("Setting CrossoverRate in MapElites") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<Default>());
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
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
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
        REQUIRE(generator->getInstanceProblem()->getName() ==
                problemCopy->getName());

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

    SECTION("Setting MutationRate in MapElites") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<Default>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
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
        REQUIRE(generator->getInstanceProblem()->getName() ==
                problemCopy->getName());

        float newMutaRate = 0.1;
        generator->setMutationRate(newMutaRate);
        REQUIRE(generator->getMutationRate() == newMutaRate);
    }

    SECTION("Setting repetitions in MapElites") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<Default>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
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
        REQUIRE(generator->getInstanceProblem()->getName() ==
                problemCopy->getName());

        int newReps = 1000;
        generator->setRepetitions(newReps);
        REQUIRE(generator->getRepetitions() == newReps);
    }

    SECTION("Setting Mutation in MapElites") {
        vector<unique_ptr<AEA>> configurations;
        configurations.push_back(make_unique<Default>());
        auto problem = make_unique<IGP>();
        auto problemCopy = make_unique<IGP>();
        unique_ptr<InstanceFitness> evaluation = make_unique<EasyInstances>();
        int generations = 1000;
        int popSize = 100;
        float mutRate = 0.05;
        float crossRate = 0.9;
        int reps = 10;
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
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
        REQUIRE(generator->getInstanceProblem()->getName() ==
                problemCopy->getName());

        unique_ptr<Mutation<IS>> mutation =
            make_unique<UniformAllMutation<IS>>();
        unique_ptr<Mutation<IS>> mutCopy =
            make_unique<UniformAllMutation<IS>>();
        generator->setMutation(move(mutation));
        REQUIRE(generator->getMutation());
    }

    SECTION("Setting Crossover in MapElites") {
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
        unique_ptr<Crossover<IS>> crossover =
            make_unique<UniformCrossover<IS>>();
        unique_ptr<Crossover<IS>> crossoverCopy =
            make_unique<UniformCrossover<IS>>();
        generator->setCrossover(move(crossover));
        REQUIRE(generator->getCrossover());
    }

    SECTION("Setting Selection in MapElites") {
        auto generator = make_unique<MapElites<IGP, IS, OP, OS>>();
        unique_ptr<Selection<IS>> selection =
            make_unique<BinaryTournamentSelection<IS>>();
        unique_ptr<Selection<IS>> selectionCopy =
            make_unique<BinaryTournamentSelection<IS>>();
        generator->setSelection(move(selection));
        REQUIRE(generator->getSelection());
    }

    SECTION("MapElites can generate KP instances") {
        using OS = BoolFloatSolution;
        using OP = KPNR;
        using IS = KPInstance;
        using IP = KPDomain;

        // MapElites Parameters
        auto generations = 10;
        auto reps = 1;
        auto meaCXRate = 0.8;
        auto nInstances = 10;
        auto easyEvaluator = make_unique<EasyInstances>();

        // KP instances parameters
        auto upperBound = 1000;
        auto instanceSize = 100;
        // EAs configurations parameters
        auto mutationRate = 1.0 / float(instanceSize);
        // Features
        auto nBins = 25;
        Features features = {
            {0, {711.0, 30000.0, nBins}}, {1, {890.0, 1000.0, nBins}},
            {2, {860.0, 1000.0, nBins}},  {3, {1.0, 200.0, nBins}},
            {4, {1.0, 230.0, nBins}},     {5, {0.10, 12.0, nBins}},
            {6, {400.0, 610.0, nBins}},   {7, {240.0, 330.0, nBins}}};

        auto instKP = make_unique<KPDomain>(instanceSize, 1, nInstances, 1,
                                            upperBound, 1, upperBound);

        // Building a portfolio of heuristics
        vector<unique_ptr<AEA>> algorithms;
        algorithms.push_back(make_unique<Default>());
        algorithms.push_back(make_unique<MPW>());
        algorithms.push_back(make_unique<MaP>());
        algorithms.push_back(make_unique<MiW>());

        // Building the MapElites
        unique_ptr<EIG<IP, IS, KPNR, OS>> generator =
            EIGBuilder<IP, IS, OP, OS>::create(GeneratorType::MapElites)
                .toSolve(move(instKP))
                .with()
                .portfolio(algorithms)
                .evalWith(move(easyEvaluator))
                .repeating(reps)
                .with()
                .crossover(CXType::Uniform)
                .mutation(MutType::UniformOne)
                .selection(SelType::Binary)
                .withMutRate(mutationRate)
                .withCrossRate(meaCXRate)
                .populationOf(nInstances)
                .runDuring(generations)
                .setFeatures(features);

        REQUIRE_FALSE(generator == nullptr);
        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == nInstances);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == (float)mutationRate);
        REQUIRE(generator->getCrossoverRate() == (float)meaCXRate);
        REQUIRE(generator->getPortfolio().size() == 4);
        REQUIRE(generator->getInstanceProblem()->getName() == "KPDomain");
        // Heavy stuff
        REQUIRE_NOTHROW(generator->run());
        REQUIRE(generator->getElapsedTime() != 0.0);
        auto front = generator->getResults();
        if (front.empty()) {
            REQUIRE(generator->getPopulation().size() == 0);
            REQUIRE(front.getNumOfSolutions() == 0);
        } else {
            REQUIRE(front.getNumOfSolutions() != 0);
        }
        // Reused the work to tests the JSON

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
        REQUIRE(data["features_info"].is_array());
    }

    SECTION("MapElites throws if no features are set") {
        using OS = BoolFloatSolution;
        using OP = KPNR;
        using IS = KPInstance;
        using IP = KPDomain;

        // MapElites Parameters
        auto generations = 2;
        auto reps = 1;
        auto meaCXRate = 0.8;
        auto nInstances = 10;
        auto easyEvaluator = make_unique<EasyInstances>();

        // KP instances parameters
        auto upperBound = 1000;
        auto instanceSize = 100;
        // EAs configurations parameters
        auto mutationRate = 1.0 / float(instanceSize);

        auto instKP = make_unique<KPDomain>(instanceSize, 1, nInstances, 1,
                                            upperBound, 1, upperBound);

        // Building a portfolio of heuristics
        vector<unique_ptr<AEA>> algorithms;
        algorithms.push_back(make_unique<Default>());
        algorithms.push_back(make_unique<MPW>());
        algorithms.push_back(make_unique<MaP>());
        algorithms.push_back(make_unique<MiW>());

        // Building the MapElites
        unique_ptr<EIG<IP, IS, KPNR, OS>> generator =
            EIGBuilder<IP, IS, OP, OS>::create(GeneratorType::MapElites)
                .toSolve(move(instKP))
                .with()
                .portfolio(algorithms)
                .evalWith(move(easyEvaluator))
                .repeating(reps)
                .with()
                .crossover(CXType::Uniform)
                .mutation(MutType::UniformOne)
                .selection(SelType::Binary)
                .withMutRate(mutationRate)
                .withCrossRate(meaCXRate)
                .populationOf(nInstances)
                .runDuring(generations);

        REQUIRE_FALSE(generator == nullptr);
        REQUIRE_THROWS(generator->run());

        REQUIRE(generator->getGenerations() == generations);
        REQUIRE(generator->getPopulationSize() == nInstances);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getMutationRate() == (float)mutationRate);
        REQUIRE(generator->getCrossoverRate() == (float)meaCXRate);
        REQUIRE(generator->getPortfolio().size() == 4);
        REQUIRE(generator->getInstanceProblem()->getName() == "KPDomain");
    }
}