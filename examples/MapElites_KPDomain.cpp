/**
 * @file MapElites_KPDomain.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-04-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/algorithms/kp_heuristics/Default.h>
#include <dignea/algorithms/kp_heuristics/MPW.h>
#include <dignea/algorithms/kp_heuristics/MaP.h>
#include <dignea/algorithms/kp_heuristics/MiW.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/generator/EIG.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/evaluations/EasyInstances.h>
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
void runExperiment(vector<unique_ptr<EA>> &algs, const int nBins = 3,
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

    auto instKP = make_unique<KPDomain>(instanceSize, 1, nInstances, 1,
                                        upperBound, 1, upperBound, false);

    string outFile = format(
        "MapElites_{}_Bins_{}_generations_KP_Domain_{}_Inst_N_{}_Target_{}_rep_"
        "{}",
        nBins, generations, nInstances, instanceSize, targetName, repetition);

    std::cout << "Filename is: " << outFile << std::endl;
    // The expected order is:
    // Q, maxP, maxW, minP, minW, avgEff, solMean, std
    // Features features = {
    //     {0, {711.0, 30000.0, nBins}}, {1, {890.0, 1000.0, nBins}},
    //     {2, {860.0, 1000.0, nBins}},  {3, {1.0, 200.0, nBins}},
    //     {4, {1.0, 230.0, nBins}},     {5, {0.10, 12.0, nBins}},
    //     {6, {400.0, 610.0, nBins}},   {7, {240.0, 330.0, nBins}}};

    // Information from features for N = 1000
    //            capacity      avg_eff        max_p        max_w         mean
    //            min_p        min_w          std
    // min     44109.000000     0.910000   993.000000   995.000000 483.033997
    // 1.000000     1.000000   278.974487 max    469903.000000     2.893500
    // 1000.000000  1000.000000   518.860474    13.000000     6.000000
    // 296.054901
    Features features = {
        {0, {44000.0, 47000.0, nBins}}, {1, {993.0, 1000.0, nBins}},
        {2, {995.0, 1000.0, nBins}},    {3, {1.0, 13.0, nBins}},
        {4, {1.0, 6.0, nBins}},         {5, {0.91, 3.0, nBins}},
        {6, {480.0, 520.0, nBins}},     {7, {270.0, 300.0, nBins}}};

    // Building the EIG
    unique_ptr<EIG<IP, IS, KPNR, OS>> generator =
        EIGBuilder<IP, IS, KPNR, OS>::create(GeneratorType::MapElites)
            .toSolve(move(instKP))
            .with()
            .portfolio(algs)
            .evalWith(move(pisPerformance))
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
    if (argc != 2) {
        std::cerr << "Error in parameters!\nUsage: ./MapElites_KP <nBins>"
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    auto nBins = stoi(argv[1]);
    auto processNumber = 0;

    vector<unique_ptr<EA>> algs;
    algs.push_back(make_unique<MaP>());
    algs.push_back(make_unique<Default>());
    algs.push_back(make_unique<MPW>());
    algs.push_back(make_unique<MiW>());
    runExperiment(algs, nBins, processNumber);

    // 4. Target --> MiW Heuristic
    algs.clear();
    algs.push_back(make_unique<MiW>());
    algs.push_back(make_unique<Default>());
    algs.push_back(make_unique<MPW>());
    algs.push_back(make_unique<MaP>());
    runExperiment(algs, nBins, processNumber);
    return EXIT_SUCCESS;
}
