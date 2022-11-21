/**
 * @file MEAKPExperiment.cpp
 * @author your name (you@domain.com)
 * @brief Experiment for generating KP instances using MEA
 * with different targets
 * @version 0.1
 * @date 2022-01-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/MEABuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/core/Front.h>
#include <dignea/mea/MEA.h>
#include <dignea/mea/evaluations/EasyInstances.h>
#include <dignea/mea/problems/IKPProblem.h>
#include <dignea/mea/solutions/IKPSolution.h>
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
using IS = IKPSolution;
using IP = IKPProblem;
using EA = AbstractEA<OS>;

int main(int argc, char **argv) {
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
    auto nInstances = 10;
    auto easyEvaluator = make_unique<EasyInstances>();

    // KP instances parameters
    auto upperBound = 1000;
    auto instanceSize = 100;
    // EAs configurations parameters
    auto maxEvals = 100000;
    auto popSize = 32;

    auto crossRates = vector<float>{};
    for (int i = 1; i < PARAMS - 2; i++) {
        crossRates.push_back(atof(argv[i]));
    }
    auto startingRep{0};  //{stoi(argv[PARAMS - 1])};
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
    auto thresholdNS = 50;
    auto k = 3;
    auto distance = make_unique<Euclidean<float>>();

    vector<unique_ptr<EA>> algorithms;
    auto instKP = make_unique<IKPProblem>(instanceSize, nInstances, 1,
                                          upperBound, 1, upperBound);

    string outFile = format(
        "MEA_KP_{}_generations_{}_PS_NSPerf_K_3_threshold_100_"
        "Euclidean_WS_{}_{}_"
        "Inst_N_{}_"
        "Target_GA32PS_CR_{}_"
        "Evals_{}",
        generations, nInstances, fitnessRatio, diversityRatio, instanceSize,
        crossRates[0], maxEvals);
    std::cout << "Filename is: " << outFile << std::endl;

    // Building the EA configurations
    for (int i = 0; i < 4; i++) {
        unique_ptr<ParallelGeneticAlgorithm<OS>> algorithm =
            ParGABuilder<OS>::create()
                .usingCores(32)
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

    // Building the MEA
    unique_ptr<MEA<IP, IS, OP, OS>> mea =
        MEABuilder<IP, IS, OP, OS>::create()
            .toSolve(move(instKP))
            .with()
            .weights(fitnessRatio, diversityRatio)
            .portfolio(algorithms)
            .evalWith(move(easyEvaluator))
            .repeating(reps)
            .withSearch(NSType::Performance, move(distance), thresholdNS, k)
            .with()
            .crossover(CXType::Uniform)
            .mutation(MutType::UniformOne)
            .selection(SelType::Binary)
            .withMutRate(mutationRate)
            .withCrossRate(meaCXRate)
            .populationOf(nInstances)
            .runDuring(generations);
    mea->run();

    // Priting the results

    auto printer = make_unique<JSONPrinter>(outFile);
    auto meaData = mea->to_json();
    auto problemData = mea->getInstanceProblem()->to_json();
    auto front = mea->getResults();
    auto frontData = front.to_json();
    printer->append("algorithm", meaData);
    printer->append("problem", problemData);
    printer->append("front", frontData);
    printer->flush();

    string kpInsPattern =
        format("Inst_N_{}_Target_GA32PS_CR_{}_{}_{}_{}_", instanceSize,
               crossRates[0], fitnessRatio, diversityRatio, startingRep);
    auto instPrinter = make_unique<InstPrinter<IS>>(kpInsPattern, ".kp");
    instPrinter->printInstances(front);

    return 0;
}
