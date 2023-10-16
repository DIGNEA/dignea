/**
 * @file TSP_Heuristics.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <dignea/algorithms/tsp_heuristics/Greedy.h>
#include <dignea/algorithms/tsp_heuristics/NearestNeighbour.h>
#include <dignea/algorithms/tsp_heuristics/TwoOpt.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/generator/EIG.h>
#include <dignea/generator/domains/TSPDomain.h>
#include <dignea/generator/evaluations/EasyInstances.h>
#include <dignea/generator/instances/TSPInstance.h>
#include <dignea/problems/TSP.h>
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
using namespace tsp_heuristics;

using OS = IntFloatSolution;
using IS = TSPInstance;
using IP = TSPDomain;
using EA = AbstractEA<OS>;

/**
 * @brief Performs an experiment to generate TSP instances using heuristics.
 *
 * @param algs
 * @param fRatio
 * @param nRatio
 */
void runExperiment(vector<unique_ptr<EA>> &algs, const float &fRatio = 0.85,
                   const float &nRatio = 0.15, const float thresholdNS = 0.25,
                   const int repetition = 0) {
    auto targetName = algs[0]->getName();

    // TSP instances parameters
    auto upperBound = 100;
    auto instanceSize = 100;
    // EIG Parameters
    auto generations = 1000;
    auto reps = 1;
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

    auto k = 5;
    auto distance = make_unique<Euclidean<float>>();
    auto instTSP = make_unique<TSPDomain>(instanceSize, 2, upperBound, 1);

    string outFile = format(
        "EIG_TSP_{}_generations_{}_PS_Performance_K_3_threshold_{}_"
        "Euclidean_WS_{}_{}_Inst_N_{}_Target_{}_rep_{}",
        generations, nInstances, thresholdNS, fRatio, nRatio, instanceSize,
        targetName, repetition);

    std::cout << "Filename is: " << outFile << std::endl;
    // Building the EIG
    unique_ptr<EIG<IP, IS, TSP, OS>> generator =
        EIGBuilder<IP, IS, TSP, OS>::create(GeneratorType::LinearScaled)
            .toSolve(move(instTSP))
            .with()
            .weights(fRatio, nRatio)
            .portfolio(algs)
            .evalWith(move(easyEvaluator))
            .repeating(reps)
            .withSearch(NSType::Performance, move(distance), thresholdNS,
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
}

/**
 * @brief Main method of the program to control de flow of the experimental
 * procedure. It performs the experiment four times using the four different TSP
 * heuristics implemented
 * 1. Nearest Neighbour
 * 2. Greedy
 * 3. NN 2-Opt
 * 4. Random 2-Opt
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Error in args. \nUsage: ./TSP_Heuristics "
                     "<repetition>"
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    auto i = stoi(argv[1]);
    float fRatio = 0.85;
    float nRatio = 0.15;
    float threshold = 0.000001;
    //    1. Target --> NN
    vector<unique_ptr<EA>> algs;
    algs.push_back(make_unique<NearestNeighbour>());
    algs.push_back(make_unique<Greedy>());
    algs.push_back(make_unique<TwoOpt>());
    algs.push_back(make_unique<TwoOpt>(false));
    runExperiment(algs, fRatio, nRatio, threshold, i);

    // 2. Target --> Greedy
    algs.clear();
    algs.push_back(make_unique<Greedy>());
    algs.push_back(make_unique<NearestNeighbour>());
    algs.push_back(make_unique<TwoOpt>());
    algs.push_back(make_unique<TwoOpt>(false));
    runExperiment(algs, fRatio, nRatio, threshold, i);

    // 3. Target --> NN 2-Opt
    algs.clear();
    algs.push_back(make_unique<TwoOpt>());
    algs.push_back(make_unique<NearestNeighbour>());
    algs.push_back(make_unique<Greedy>());
    algs.push_back(make_unique<TwoOpt>(false));

    runExperiment(algs, fRatio, nRatio, threshold, i);

    // 3. Target --> Random 2-Opt
    algs.clear();
    algs.push_back(make_unique<TwoOpt>(false));
    algs.push_back(make_unique<NearestNeighbour>());
    algs.push_back(make_unique<Greedy>());
    algs.push_back(make_unique<TwoOpt>());

    runExperiment(algs, fRatio, nRatio, threshold, i);

    return EXIT_SUCCESS;
}
