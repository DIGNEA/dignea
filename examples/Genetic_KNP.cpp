/**
 * @file Genetic_KNP.cpp
 * @author your name (you@domain.com)
 * @brief Code for solving all the KP instances in a specific directory with one
 * GA configuration
 * @version 0.1
 * @date 27/11/19.
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/algorithms/kp_heuristics/Default.h>
#include <dignea/algorithms/kp_heuristics/ExpKnap.h>
#include <dignea/algorithms/kp_heuristics/MPW.h>
#include <dignea/algorithms/kp_heuristics/MaP.h>
#include <dignea/algorithms/kp_heuristics/MiW.h>
#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/builders/ExpBuilder.h>
#include <dignea/builders/ParGABuilder.h>
#include <dignea/core/AbstractEA.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/problems/KPNR.h>
#include <dignea/utilities/printer/JSONPrinter.h>
#include <fmt/core.h>

#include <array>
#include <filesystem>
#include <iostream>
#include <string>
using namespace std;
namespace fs = std::filesystem;

using S = BoolFloatSolution;
using P = Problem<S>;
using EA = AbstractEA<S>;
using GA = ParallelGeneticAlgorithm<S>;
using ParGA = ParallelGeneticAlgorithm<S>;

int main(int argc, char **argv) {
    auto nArgs = 5;
    if (argc != nArgs) {
        cerr << "Error in args. <path-to-instances> <path-to-results> "
                "<crossrate> <pop-size>"
             << endl;
        exit(EXIT_FAILURE);
    }
    // Parametros del experimento
    auto evaluation = 1e4;
    auto cores = 32;
    string pathToInstance(argv[1]);
    string pathToResults(argv[2]);
    float crossRate = stof(argv[3]);
    auto popsize = stoi(argv[4]);

    for (auto &entry : fs::recursive_directory_iterator(pathToInstance)) {
        if (!entry.is_directory()) {
            // Problema a resolver
            unique_ptr<P> problem = make_unique<KPNR>(entry.path());
            unique_ptr<P> expProblem = make_unique<KPNR>(entry.path());
            std::cout << "Instance: " << entry.path() << std::endl;

            // Probabilidad de cruce como 1 / N.
            double mutRate = 1.0 / 1000;  // El instanceSize en MEA es N * 2
            string outputFile = "GA_" + fmt::format("{:.2f}", crossRate) + "_" +
                                entry.path().filename().string();
            unique_ptr<ParGA> algorithm = ParGABuilder<S>::create()
                                              .usingCores(cores)
                                              .toSolve(move(problem))
                                              .with()
                                              .crossover(CXType::Uniform)
                                              .mutation(MutType::UniformOne)
                                              .selection(SelType::Binary)
                                              .withMutRate(mutRate)
                                              .withCrossRate(crossRate)
                                              .with()
                                              .populationOf(popsize)
                                              .runDuring(evaluation);

            algorithm->run();
            std::cout << setw(2) << algorithm->getResults().to_json()
                      << std::endl;
            // unique_ptr<Experiment<S>> experiment =
            //     ExpBuilder<S>::create(outputFile)
            //         .saveResultsIn(pathToResults)
            //         .usingAlgorithm(move(algorithm))
            //         .toSolve(move(expProblem))
            //         .repeat(repetitions);
            // experiment->run();
            // json data = experiment->to_json();
            // auto printer = make_unique<JSONPrinter>("printer");
            // printer->print(outputFile, data);

            // unique_ptr<EA> heuristic = make_unique<ExpKnap>();
            // heuristic->setProblem(problem.get());
            // heuristic->run();
            // std::cout << "OK" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
