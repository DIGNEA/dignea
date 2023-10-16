/**
 * @file BPP_Heuristics.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <dignea/algorithms/bpp_heuristics/BestFit.h>
#include <dignea/algorithms/bpp_heuristics/FirstFit.h>
#include <dignea/algorithms/bpp_heuristics/NextFit.h>
#include <dignea/algorithms/bpp_heuristics/WorstFit.h>
#include <dignea/builders/EIGBuilder.h>
#include <dignea/core/Front.h>
#include <dignea/generator/EIG.h>
#include <dignea/generator/domains/BPPDomain.h>
#include <dignea/generator/evaluations/EasyInstances.h>
#include <dignea/generator/instances/BPPInstance.h>
#include <dignea/problems/BPP.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/printer/JSONPrinter.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <array>
#include <filesystem>
#include <string>

using namespace fmt;
using namespace std;
namespace fs = std::filesystem;

using OS = IntIntSolution;
using IS = BPPInstance;
using IP = BPPDomain;
using EA = AbstractEA<OS>;
using P = Problem<OS>;

std::array<float, 4> runSolvers(shared_ptr<BPP> instance) {
    auto bestFit = make_unique<BestFit>();
    auto worstFit = make_unique<WorstFit>();
    auto nextFit = make_unique<NextFit>();
    auto firstFit = make_unique<FirstFit>();

    bestFit->setProblem(instance);
    worstFit->setProblem(instance);
    nextFit->setProblem(instance);
    firstFit->setProblem(instance);

    bestFit->run();
    worstFit->run();
    nextFit->run();
    firstFit->run();

    auto bestFitness = bestFit->getResults().getSolutions()[0].getFitness();
    auto worstFitness = worstFit->getResults().getSolutions()[0].getFitness();
    auto nextFitness = nextFit->getResults().getSolutions()[0].getFitness();
    auto firstFitness = firstFit->getResults().getSolutions()[0].getFitness();

    std::array<float, 4> results = {bestFitness, worstFitness, nextFitness,
                                    firstFitness};
    return results;
}

void solveInstance(const string &pathToInstance) {
    // Parametros del experimento
    auto entry = fs::path(pathToInstance);
    auto problem = make_shared<BPP>(entry);
    std::cout << "Instance: " << entry.filename() << std::endl;
    std::cout << "Number of items: " << problem->getNumberOfVars() << std::endl;

    auto [best, worst, next, first] = runSolvers(problem);

    string outFile = format("Instance_{}_results", entry.filename().string());
    auto printer = make_unique<JSONPrinter>(outFile);
    json instanceResults;
    instanceResults["BestFit"] = best;
    instanceResults["WorstFit"] = worst;
    instanceResults["NextFit"] = next;
    instanceResults["FirstFit"] = first;

    std::cout << instanceResults << std::endl;
    printer->print(outFile, instanceResults);
}

/**
 * @brief Performs an experiment to generate BPP instances using heuristics.
 *
 * @param algs
 * @param fRatio
 * @param nRatio
 */
void runExperiment(vector<unique_ptr<EA>> &algs, const float &fRatio = 0.85,
                   const float &nRatio = 0.15, const float thresholdNS = 0.25,
                   const int repetition = 0) {
    auto targetName = algs[0]->getName();

    // BPP instances parameters
    auto upperBound = 100;
    auto lowerBound = 20;
    auto instanceSize = 120;
    auto capacity = 150;
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
    auto k = 3;
    float thresholdFinal = 1e-7;

    auto distance = make_unique<Euclidean<float>>();
    auto instBPP = make_unique<BPPDomain>(instanceSize, 1, capacity, lowerBound,
                                          upperBound);

    string outFile = format(
        "EIG_BPP_{}_generations_first_improve_{}_PS_NSFeatures_K_3_threshold_{}"
        "_"
        "Euclidean_WS_{}_{}_Inst_N_{}_Target_{}_rep_{}",
        generations, nInstances, thresholdNS, fRatio, nRatio, instanceSize,
        targetName, repetition);

    std::cout << "Filename is: " << outFile << std::endl;
    bool warmUpNS = false;

    // Building the EIG
    unique_ptr<EIG<IP, IS, BPP, OS>> generator =
        EIGBuilder<IP, IS, BPP, OS>::create(GeneratorType::LinearScaled)
            .toSolve(move(instBPP))
            .with()
            .weights(fRatio, nRatio)
            .portfolio(algs)
            .evalWith(move(easyEvaluator))
            .repeating(reps)
            .withSearch(NSType::Features, move(distance), thresholdNS,
                        thresholdFinal, k, warmUpNS)
            .with()
            .crossover(CXType::Uniform)
            .mutation(MutType::UniformOne)
            .selection(SelType::Binary)
            .replacement(ReplacementTypes::FirstImprove)
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

int main(int argc, char **argv) {
    float fRatio = 0.6;
    float nRatio = 0.4;
    float threshold = 1e-3;
    auto processNumber = 0;

    //    1. Target --> Best Heuristic
    vector<unique_ptr<EA>> algs;
    algs.push_back(make_unique<BestFit>());
    algs.push_back(make_unique<WorstFit>());
    algs.push_back(make_unique<NextFit>());
    algs.push_back(make_unique<FirstFit>());
    runExperiment(algs, fRatio, nRatio, threshold, processNumber);

    // 2. Target --> Worst Heuristic
    algs.clear();
    algs.push_back(make_unique<WorstFit>());
    algs.push_back(make_unique<BestFit>());
    algs.push_back(make_unique<NextFit>());
    algs.push_back(make_unique<FirstFit>());
    runExperiment(algs, fRatio, nRatio, threshold, processNumber);

    // 3. Target --> Next Heuristic
    algs.clear();
    algs.push_back(make_unique<NextFit>());
    algs.push_back(make_unique<BestFit>());
    algs.push_back(make_unique<WorstFit>());
    algs.push_back(make_unique<FirstFit>());
    runExperiment(algs, fRatio, nRatio, threshold, processNumber);

    // 4. Target --> First Heuristic
    algs.clear();
    algs.push_back(make_unique<FirstFit>());
    algs.push_back(make_unique<NextFit>());
    algs.push_back(make_unique<BestFit>());
    algs.push_back(make_unique<WorstFit>());
    runExperiment(algs, fRatio, nRatio, threshold, processNumber);

    return EXIT_SUCCESS;
}
