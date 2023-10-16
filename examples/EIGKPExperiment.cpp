/**
 * @file EIGKPExperiment.cpp
 * @author your name (you@domain.com)
 * @brief Experiment for generating KP instances using EIG
 * with different targets
 * @version 0.1
 * @date 2022-01-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/core/Front.h>
#include <dignea/generator/EIG.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/evaluations/EasyInstances.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/problems/KPNR.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/printer/InstPrinter.h>
#include <dignea/utilities/printer/JSONPrinter.h>
#include <fmt/core.h>

#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using namespace fmt;
using json = nlohmann::json;
using OS = BoolFloatSolution;
using OP = KPNR;
using IS = KPInstance;
using IP = KPDomain;
using EA = AbstractEA<OS>;

int main(int argc, char **argv) {
    auto PARAMS = 8;
    if (argc != PARAMS) {
        std::cerr << "Error in parameters. Expected : <cx1> <cx2> <cx3> <cx4> "
                     "<fr> <nr> <rep>"
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // EIG Parameters
    auto generations = 1000;
    auto reps = 10;
    auto meaCXRate = 0.8;
    auto nInstances = 10;
    auto easyEvaluator = make_unique<EasyInstances>();

    // KP instances parameters
    auto upperBound = 1000;
    auto instanceSize = 100;  // Generamos instancias de N = 1000
    // EAs configurations parameters
    auto maxEvals = 10000;  // 10000 evaluaciones segun estudio
    auto popSize = 32;

    // Portfolio parameters
    auto cores = 32;
    auto crossRates = vector<float>{};
    for (int i = 1; i < PARAMS - 2; i++) {
        crossRates.push_back(atof(argv[i]));
    }
    auto startingRep{stoi(argv[PARAMS - 1])};
    float diversityRatio{stof(argv[PARAMS - 2])};
    float fitnessRatio{stof(argv[PARAMS - 3])};

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

    vector<unique_ptr<EA>> algorithms;
    auto instKP = make_unique<KPDomain>(instanceSize, 1, nInstances, 1,
                                        upperBound, 1, upperBound);

    string outFile = format(
        "EIG_KP_{}_generations_{}_PS_NSFeatures_K_3_threshold_{}_"
        "Euclidean_WS_{}_{}_"
        "Inst_N_{}_"
        "Target_GA_32_PS_CR_{}_"
        "Evals_{}_rep_{}",
        generations, nInstances, thresholdNS, fitnessRatio, diversityRatio,
        instanceSize, crossRates[0], maxEvals, startingRep);
    std::cout << "Filename is: " << outFile << std::endl;

    // Building the EA configurations
    for (int i = 0; i < 4; i++) {
        unique_ptr<ParallelGeneticAlgorithm<OS>> algorithm =
            ParGABuilder<OS>::create()
                .usingCores(cores)
                .with()
                .mutation(MutType::UniformOne)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .populationOf(popSize)
                .withMutRate(mutationRate)
                .withCrossRate(crossRates[i])
                .runDuring(maxEvals);
        algorithms.push_back(move(algorithm));
    }

    // Building the EIG
    unique_ptr<EIG<IP, IS, OP, OS>> generator =
        EIGBuilder<IP, IS, OP, OS>::create(GeneratorType::LinearScaled)
            .toSolve(move(instKP))
            .with()
            .weights(fitnessRatio, diversityRatio)
            .portfolio(algorithms)
            .evalWith(move(easyEvaluator))
            .repeating(reps)
            .withSearch(NSType::Features, move(distance), thresholdNS,
                        thresholdNS, k)
            .with()
            .crossover(CXType::Uniform)
            .mutation(MutType::UniformOne)
            .selection(SelType::Binary)
            .withMutRate(mutationRate)
            .withCrossRate(meaCXRate)
            .populationOf(nInstances)
            .runDuring(generations);
    generator->run();

    // Priting the results

    auto printer = make_unique<JSONPrinter>(outFile);
    auto meaData = generator->to_json();
    auto problemData = generator->getInstanceProblem()->to_json();
    auto front = generator->getResults();
    auto frontData = front.to_json();
    printer->append("algorithm", meaData);
    printer->append("problem", problemData);
    printer->append("front", frontData);
    printer->flush();

    return 0;
}
