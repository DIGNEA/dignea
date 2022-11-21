/**
 * @file main.cpp
 * @author Alejandro Marrero -  amarrerd@ull.edu.es
 * @brief Experiment for generating TSP instances using MEA
 * with different targets
 * @version 0.1
 * @date 2022-04-26
 *
 * @copyright Copyright (c) 2022
 *
 */

//#define DEBUG

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/MEABuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/core/Front.h>
#include <dignea/crossovers/OrderCrossover.h>
#include <dignea/mea/MEA.h>
#include <dignea/mea/evaluations/EasyInstances.h>
#include <dignea/mea/problems/ITSPProblem.h>
#include <dignea/mea/solutions/ITSPSolution.h>
#include <dignea/problems/TSP.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/printer/InstPrinter.h>
#include <dignea/utilities/printer/JSONPrinter.h>
#include <fmt/core.h>

#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using namespace fmt;
using json = nlohmann::json;
using OSS = IntFloatSolution;
using OP = TSP;
using IS = ITSPSolution;
using IP = ITSPProblem;
using EA = AbstractEA<OSS>;

int main(int argc, char** argv) {
    auto PARAMS = 7;
    if (argc != PARAMS) {
        std::cerr << "Error in parameters. Expected 4 crossover rates and a "
                     "fitness_ratio and diversity_ratio"
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // MEA Parameters
    auto generations = 1000;
    auto reps = 10;
    auto meaCXRate = 0.8;
    auto nInstances = 32;
    auto easyEvaluator = make_unique<EasyInstances>();

    // KP instances parameters
    auto upperBound = 100.0f;
    auto lowerBound = 0.0f;
    auto instanceSize = 100;
    // EAs configurations parameters
    auto maxEvals = 1e5;

    auto crossRates = vector<float>{};
    for (int i = 1; i < PARAMS - 2; i++) {
        crossRates.push_back(atof(argv[i]));
    }

    float fitnessRatio{stof(argv[PARAMS - 2])};
    float diversityRatio{stof(argv[PARAMS - 1])};
    if (fitnessRatio + diversityRatio != 1.0) {
        std::cerr << "Error in ratios. Fitness and diversity ratios must sum "
                     "up to 1.0"
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    auto mutationRate = 1.0 / float(instanceSize);

    // Novelty Search parameters
    auto thresholdNS = 3;
    auto k = 3;
    auto distance = make_unique<Euclidean<float>>();

    // ITSPProblem with 100 cities and bounds [0.0, 100.0]
    auto instTSP = make_unique<IP>(instanceSize, upperBound, lowerBound);
    vector<unique_ptr<EA>> algorithms;

    // Building the EA configurations
    for (float& cxRate : crossRates) {
        unique_ptr<ParallelGeneticAlgorithm<OSS>> algorithm =
            ParGABuilder<OSS>::create()
                .usingCores(nInstances)
                .with()
                .mutation(MutType::SwapMutation)
                .crossover(CXType::Order)
                .selection(SelType::Binary)
                .populationOf(32)
                .withMutRate(mutationRate)
                .withCrossRate(cxRate)
                .runDuring(maxEvals);
        algorithms.push_back(move(algorithm));
    }

    // Building the MEA
    unique_ptr<MEA<IP, IS, OP, OSS>> mea =
        MEABuilder<IP, IS, OP, OSS>::create()
            .toSolve(move(instTSP))
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

    mea->run();

    // Printing the results
    string outFile = format(
        "MEA_TSP_combined_front_{}_generations_{}_PS_NS_K_3_threshold_3_"
        "Euclidean_WS_{}_{}_"
        "Inst_N_{}_"
        "Target_GA32PS_CR_{}_"
        "Evals_{}",
        generations, nInstances, fitnessRatio, diversityRatio, instanceSize,
        crossRates[0], maxEvals);
    std::cout << "Filename is: " << outFile << std::endl;

    auto printer = make_unique<JSONPrinter>(outFile);
    auto meaData = mea->to_json();
    auto problemData = mea->getInstanceProblem()->to_json();
    auto front = mea->getResults();
    auto frontData = front.to_json();
    printer->append("algorithm", meaData);
    printer->append("problem", problemData);
    printer->append("front", frontData);
    printer->flush();
    auto tspNamePattern =
        format("Inst_N_{}_Target_GA32PS_CR_{}_{}_{}_", instanceSize,
               crossRates[0], fitnessRatio, diversityRatio);
    auto instPrinter = make_unique<InstPrinter<IS>>(tspNamePattern, ".tsp");
    instPrinter->printInstances(front);
    return 0;
}