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
#include <dignea/algorithms/kp_heuristics/Default.h>
#include <dignea/algorithms/kp_heuristics/MPW.h>
#include <dignea/algorithms/kp_heuristics/MaP.h>
#include <dignea/algorithms/kp_heuristics/MiW.h>
#include <dignea/builders/MEABuilder.h>
#include <dignea/mea/MEA.h>
#include <dignea/mea/evaluations/EasyInstances.h>
#include <dignea/mea/problems/IKPProblem.h>
#include <dignea/mea/solutions/IKPSolution.h>
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
using IS = IKPSolution;
using IP = IKPProblem;
using EA = AbstractEA<OS>;

/**
 * @brief Performs an experiment to generate KP instances using heuristics.
 *
 * @param algs
 * @param fRatio
 * @param nRatio
 */
void runExperiment(vector<unique_ptr<EA>> &algs, const float &fRatio = 0.85,
                   const float &nRatio = 0.15, const int repetition = 0) {
    auto targetName = algs[0]->getName();

    // KP instances parameters
    auto upperBound = 1000;
    auto instanceSize = 100;
    // MEA Parameters
    auto generations = 1000;
    auto reps = 10;
    auto meaCXRate = 0.8;
    auto nInstances = 10;
    auto easyEvaluator = make_unique<EasyInstances>();
    auto mutationRate = 1.0 / float(instanceSize);
    if (fRatio + nRatio != 1.0) {
        std::cerr << "Error in ratios. Fitness and diversity ratios must sum "
                     "up to 1.0"
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // Novelty Search parameters
    auto thresholdNS = 3;
    auto k = 3;
    auto distance = make_unique<Euclidean<float>>();

    auto instKP = make_unique<IKPProblem>(instanceSize, nInstances, 1,
                                          upperBound, 1, upperBound);

    string outFile = format(
        "MEA_KP_{}_generations_{}_PS_NSFeatures_K_3_threshold_3_"
        "Euclidean_WS_{}_{}_Inst_N_{}_Target_{}_rep_{}",
        generations, nInstances, fRatio, nRatio, instanceSize, targetName,
        repetition);

    std::cout << "Filename is: " << outFile << std::endl;

    // Building the MEA
    unique_ptr<MEA<IP, IS, KPNR, OS>> mea =
        MEABuilder<IP, IS, KPNR, OS>::create()
            .toSolve(move(instKP))
            .with()
            .weights(fRatio, nRatio)
            .portfolio(algs)
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

    string kpInsPattern = format("Inst_N_{}_Target_{}_{}_{}_{}", instanceSize,
                                 targetName, fRatio, nRatio, repetition);
    auto instPrinter = make_unique<InstPrinter<IS>>(kpInsPattern, ".kp");
    instPrinter->printInstances(front);
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
    auto repetitions{10};
    float fRatio = 0.85;
    float nRatio = 0.15;
    for (int i = 1; i <= repetitions; i++) {
        // 1. Target --> Default Heuristic
        vector<unique_ptr<EA>> algs;
        algs.push_back(make_unique<Default>());
        algs.push_back(make_unique<MPW>());
        algs.push_back(make_unique<MaP>());
        algs.push_back(make_unique<MiW>());
        runExperiment(algs, fRatio, nRatio, i);

        // 2. Target --> MPW Heuristic
        algs.clear();
        algs.push_back(make_unique<MPW>());
        algs.push_back(make_unique<Default>());
        algs.push_back(make_unique<MaP>());
        algs.push_back(make_unique<MiW>());
        runExperiment(algs, fRatio, nRatio, i);

        // 3. Target --> MaP Heuristic
        algs.clear();
        algs.push_back(make_unique<MaP>());
        algs.push_back(make_unique<Default>());
        algs.push_back(make_unique<MPW>());
        algs.push_back(make_unique<MiW>());
        runExperiment(algs, fRatio, nRatio, i);

        // 4. Target --> MiW Heuristic
        algs.clear();
        algs.push_back(make_unique<MiW>());
        algs.push_back(make_unique<Default>());
        algs.push_back(make_unique<MPW>());
        algs.push_back(make_unique<MaP>());
        runExperiment(algs, fRatio, nRatio, i);
    }
    return EXIT_SUCCESS;
}
