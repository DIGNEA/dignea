//
// Created by amarrero on 23/6/21.
//

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/MEABuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/mea/evaluations/EasyInstances.h>
#include <dignea/mea/problems/IKPProblem.h>
#include <dignea/mea/solutions/IKPSolution.h>
#include <dignea/problems/KPNR.h>
#include <dignea/searches/NSFeatures.h>
#include <dignea/types/SolutionTypes.h>

#include <catch2/catch_all.hpp>

TEST_CASE("MEABuilder tests", "[MEABuilder]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);
    using OS = BoolFloatSolution;
    using OP = KPNR;
    using IS = IKPSolution;
    using IP = IKPProblem;
    using EA = AbstractEA<OS>;
    const int maxEvals = 1e5;
    const int nInstances = 100;
    const int popSize = 32;
    vector<float> crossRates = {0.8, 0.9, 0.7};
    const int instanceSize = 100;
    const float mutationRate = 1.0 / float(instanceSize);
    const float meaCXRate = 0.8;
    const int upperBound = 1000;
    const int nIterationsMEA = 1000;
    const int reps = 10;
    SECTION("Creates a MEA for KNP") {
        // configurations
        vector<unique_ptr<EA>> algorithms;
        auto instKP = make_unique<IKPProblem>(instanceSize, nInstances, 1,
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
            // Two configurations for MEA
            algorithms.push_back(move(algorithm));
        }
        unique_ptr<InstanceFitness> easyEvaluator =
            make_unique<EasyInstances>();

        unique_ptr<MEA<IP, IS, OP, OS>> mea =
            MEABuilder<IP, IS, OP, OS>::create()
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
                .runDuring(nIterationsMEA);
        REQUIRE(mea);
        REQUIRE(mea->getSelection()->getName() ==
                "Binary Tournament Selection");
        REQUIRE(mea->getCrossover()->getName() == "Uniform One Crossover");
        REQUIRE(mea->getMutation()->getName() == "Uniform One Mutation");
        REQUIRE(mea->getPopulationSize() == nInstances);
        REQUIRE(mea->getGenerations() == nIterationsMEA);
        REQUIRE(mea->getMutationRate() == mutationRate);
        REQUIRE(mea->getCrossoverRate() == meaCXRate);
        REQUIRE(mea->getRepetitions() == reps);
        REQUIRE(mea->getPortfolio().size() == 3);
    }

    SECTION("Throws exception if component is missing") {
        // configurations
        vector<unique_ptr<EA>> algorithms;
        auto instKP = make_unique<IKPProblem>(instanceSize, nInstances, 1,
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
            // Two configurations for MEA
            algorithms.push_back(move(algorithm));
        }
        unique_ptr<InstanceFitness> easyEvaluator =
            make_unique<EasyInstances>();

        REQUIRE_THROWS(MEABuilder<IP, IS, OP, OS>::create()
                           .toSolve(move(instKP))
                           .with()
                           .crossover(CXType::Uniform)
                           .selection(SelType::Binary)
                           .withMutRate(mutationRate)
                           .withCrossRate(meaCXRate)
                           .populationOf(nInstances)
                           .runDuring(nIterationsMEA)
                           .
                           operator unique_ptr<MEA<IP, IS, OP, OS>>());
    }
    std::cout.clear();
    std::cerr.clear();
}