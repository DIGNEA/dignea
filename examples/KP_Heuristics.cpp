/**
 * @file KP_Heuristics.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-06-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <dignea/algorithms/kp_heuristics/Combo.h>
#include <dignea/algorithms/kp_heuristics/Default.h>
#include <dignea/algorithms/kp_heuristics/ExpKnap.h>
#include <dignea/algorithms/kp_heuristics/MPW.h>
#include <dignea/algorithms/kp_heuristics/MaP.h>
#include <dignea/algorithms/kp_heuristics/MiW.h>
#include <dignea/algorithms/kp_heuristics/MinKnap.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/generator/EIG.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/evaluations/PisPerformance.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/problems/KPNR.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/printer/InstPrinter.h>
#include <dignea/utilities/printer/JSONPrinter.h>
#include <fmt/core.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
namespace fs = std::filesystem;
using namespace fmt;

using OS = BoolFloatSolution;
using IS = KPInstance;
using IP = KPDomain;
using EA = AbstractEA<OS>;

/**
 * @brief Performs an experiment to generate KP instances using heuristics.
 *
 * @param algs
 * @param fRatio
 * @param nRatio
 */
void runExperiment(vector<unique_ptr<EA>> &algs, const float &fRatio = 0.85,
                   const float &nRatio = 0.15, const float thresholdNS = 0.25,
                   const int repetition = 0) {
    auto targetName = algs[0]->getName();

    // KP instances parameters
    auto upperBound = 1000;
    auto instanceSize = 2000;
    // EIG Parameters
    auto generations = 1000;
    auto reps = 1;
    auto meaCXRate = 0.8;
    auto nInstances = 10;
    auto pisPerformance = make_unique<PisPerformance>();
    auto mutationRate = 1.0 / float(instanceSize);
    if (fRatio + nRatio != 1.0) {
        std::cerr << "Error in ratios. Fitness and diversity ratios must sum "
                     "up to 1.0"
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // Novelty Search parameters
    auto k = 3;
    float thresholdFinal = 1e-7;
    auto distance = make_unique<Euclidean<float>>();
    auto reducedSpace = false;
    auto instKP =
        make_unique<KPDomain>(instanceSize, 1, nInstances, 1, upperBound, 1,
                              upperBound, reducedSpace);

    string outFile = format(
        "EIG_KP_{}_generations_{}_PS_NSFeatures_K_3_threshold_{}_"
        "Euclidean_WS_{}_{}_Inst_N_{}_Target_{}_rep_{}",
        generations, nInstances, thresholdNS, fRatio, nRatio, instanceSize,
        targetName, repetition);

    std::cout << "Filename is: " << outFile << std::endl;
    bool warmUpNS = false;

    // Building the EIG
    unique_ptr<EIG<IP, IS, KPNR, OS>> generator =
        EIGBuilder<IP, IS, KPNR, OS>::create(GeneratorType::LinearScaled)
            .toSolve(move(instKP))
            .with()
            .weights(fRatio, nRatio)
            .portfolio(algs)
            .evalWith(move(pisPerformance))
            .repeating(reps)
            .withSearch(NSType::Features, move(distance), thresholdNS,
                        thresholdFinal, k, warmUpNS)
            .with()
            .crossover(CXType::Uniform)
            .mutation(MutType::UniformAll)
            .selection(SelType::Binary)
            .replacement(ReplacementTypes::Generational)
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
}

/**
 * @brief Main method of the program to control de flow of the experimental
 * procedure. It performs the experiment four times using the four different KP
 * heuristics implemented
 * 1. Default Heuristic
 * 2. Maximum Profit per Weight Ratio Heuristic
 * 3. Maximum Profit Heuristic
 * 4. Minimum Weight Heuristic
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv) {
    auto fRatio = 0.15;
    auto nRatio = 0.85;
    auto threshold = 1e-5;
    auto processNumber = 0;

    vector<unique_ptr<EA>> algs;
    algs.push_back(make_unique<ExpKnap>());
    algs.push_back(make_unique<MinKnap>());
    algs.push_back(make_unique<Combo>());
    runExperiment(algs, fRatio, nRatio, threshold, processNumber);

    // 2. Target --> MPW Heuristic
    algs.clear();
    algs.push_back(make_unique<MinKnap>());
    algs.push_back(make_unique<ExpKnap>());
    algs.push_back(make_unique<Combo>());
    runExperiment(algs, fRatio, nRatio, threshold, processNumber);

    algs.clear();
    algs.push_back(make_unique<Combo>());
    algs.push_back(make_unique<ExpKnap>());
    algs.push_back(make_unique<MinKnap>());
    runExperiment(algs, fRatio, nRatio, threshold, processNumber);

    // MPI_Finalize();

    return EXIT_SUCCESS;
}