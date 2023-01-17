//
// Created by amarrero on 23/6/21.
//

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/generator/evaluations/EasyInstances.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/problems/KPNR.h>
#include <dignea/searches/NSFeatures.h>
#include <dignea/types/SolutionTypes.h>

#include <catch2/catch_all.hpp>

TEST_CASE("EIGBuilder tests", "[EIGBuilder]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);
    using OS = BoolFloatSolution;
    using OP = KPNR;
    using IS = KPInstance;
    using IP = KPDomain;
    using EA = AbstractSolver<OS>;
    const int maxEvals = 1e5;
    const int nInstances = 100;
    const int popSize = 32;
    vector<float> crossRates = {0.8, 0.9, 0.7};
    const int instanceSize = 100;
    const float mutationRate = 1.0 / float(instanceSize);
    const float meaCXRate = 0.8;
    const int upperBound = 1000;
    const int nIterationsEIG = 1000;
    const int reps = 10;
    SECTION("Creates a EIG for KNP") {
        // configurations
        vector<unique_ptr<EA>> algorithms;
        auto instKP = make_unique<KPDomain>(instanceSize, nInstances, 1,
                                              upperBound, 1, upperBound);
        for (const float cRate : crossRates) {
            unique_ptr<ParallelGeneticAlgorithm<OS>> algorithm =
                ParGABuilder<OS>::create()
                    .usingCores(32)
                    .with()
                    .mutation(MutType::UniformOne)
                    .crossover(CXType::Uniform)
                    .selection(SelType::Binary)
                    .populationOf(popSize)
                    .withMutRate(mutationRate)
                    .withCrossRate(cRate)
                    .runDuring(maxEvals);
            // Two configurations for EIG
            algorithms.push_back(move(algorithm));
        }
        unique_ptr<InstanceFitness> easyEvaluator =
            make_unique<EasyInstances>();

        unique_ptr<EIG<IP, IS, OP, OS>> generator =
            EIGBuilder<IP, IS, OP, OS>::create()
                .toSolve(move(instKP))
                .with()
                .portfolio(algorithms)
                .evalWith(move(easyEvaluator))
                .repeating(reps)
                .withSearch(NSType::Features, make_unique<Euclidean<float>>(),
                            3.0, 15)
                .with()
                .crossover(CXType::Uniform)
                .mutation(MutType::UniformOne)
                .selection(SelType::Binary)
                .withMutRate(mutationRate)
                .withCrossRate(meaCXRate)
                .populationOf(nInstances)
                .runDuring(nIterationsEIG);
        REQUIRE(generator);
        REQUIRE(generator->getSelection()->getName() ==
                "Binary Tournament Selection");
        REQUIRE(generator->getCrossover()->getName() == "Uniform One Crossover");
        REQUIRE(generator->getMutation()->getName() == "Uniform One Mutation");
        REQUIRE(generator->getPopulationSize() == nInstances);
        REQUIRE(generator->getGenerations() == nIterationsEIG);
        REQUIRE(generator->getMutationRate() == mutationRate);
        REQUIRE(generator->getCrossoverRate() == meaCXRate);
        REQUIRE(generator->getRepetitions() == reps);
        REQUIRE(generator->getPortfolio().size() == 3);
    }

    SECTION("Throws exception if component is missing") {
        // configurations
        vector<unique_ptr<EA>> algorithms;
        auto instKP = make_unique<KPDomain>(instanceSize, nInstances, 1,
                                              upperBound, 1, upperBound);
        for (const float cRate : crossRates) {
            unique_ptr<ParallelGeneticAlgorithm<OS>> algorithm =
                ParGABuilder<OS>::create()
                    .usingCores(32)
                    .with()
                    .mutation(MutType::UniformOne)
                    .crossover(CXType::Uniform)
                    .selection(SelType::Binary)
                    .populationOf(popSize)
                    .withMutRate(mutationRate)
                    .withCrossRate(cRate)
                    .runDuring(maxEvals);
            // Two configurations for EIG
            algorithms.push_back(move(algorithm));
        }
        unique_ptr<InstanceFitness> easyEvaluator =
            make_unique<EasyInstances>();

        REQUIRE_THROWS(EIGBuilder<IP, IS, OP, OS>::create()
                           .toSolve(move(instKP))
                           .with()
                           .crossover(CXType::Uniform)
                           .selection(SelType::Binary)
                           .withMutRate(mutationRate)
                           .withCrossRate(meaCXRate)
                           .populationOf(nInstances)
                           .runDuring(nIterationsEIG)
                           .
                           operator unique_ptr<EIG<IP, IS, OP, OS>>());
    }
    std::cout.clear();
    std::cerr.clear();
}