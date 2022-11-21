
/**
 * @file GeneticTSP.cpp
 * @author Alejandro Marrero
 * @brief Simple Steady GA - TSP experiment test
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/builders/ExpBuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/core/Front.h>
#include <dignea/crossovers/OrderCrossover.h>
#include <dignea/problems/TSP.h>
#include <dignea/utilities/printer/JSONPrinter.h>

#include <array>
#include <string>
using namespace std;

int main(int argc, char** argv) {
    auto nArgs = 2;
    if (argc != nArgs) {
        cerr << "Error in args. <path-to-instance>" << endl;
        exit(EXIT_FAILURE);
    }
    // Parametros del experimento
    auto popSize = 32;
    double mutationRate = 1 / 100.0;
    auto repetitions{10};
    float evaluations = 1e5;
    std::array cRates = {0.7, 0.8, 0.9, 1.0};
    string pathToInstance = (argv[1]);

    for (auto& crossRate : cRates) {
        unique_ptr<TSP> tsp = make_unique<TSP>(pathToInstance);
        unique_ptr<TSP> expTSP = make_unique<TSP>(pathToInstance);
        unique_ptr<ParallelGeneticAlgorithm<IntFloatSolution>> ga =
            ParGABuilder<IntFloatSolution>::create()
                .toSolve(move(tsp))
                .with()
                .populationOf(popSize)
                .with()
                .mutation(MutType::SwapMutation)
                .crossover(CXType::Order)
                .selection(SelType::Binary)
                .withMutRate(mutationRate)
                .withCrossRate(crossRate)
                .runDuring(evaluations)
                .usingCores(32);

        auto outputFile = "ParGA_TSP_cx_" + to_string(crossRate);
        unique_ptr<Experiment<IntFloatSolution>> experiment =
            ExpBuilder<IntFloatSolution>::create(outputFile)
                .saveResultsIn("/home/amarrero/dignea/bin")
                .usingAlgorithm(move(ga))
                .toSolve(move(expTSP))
                .repeat(repetitions);
        experiment->run();
        json data = experiment->to_json();
        auto printer = make_unique<JSONPrinter>("printer");
        printer->print(outputFile, data);
        std::cout << "Experiment done" << std::endl;
    }

    return EXIT_SUCCESS;
}
