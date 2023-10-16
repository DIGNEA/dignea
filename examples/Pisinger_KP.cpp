/**
 * @file Pisinger_KP.cpp
 * @author your name (you@domain.com)
 * @brief Code for solving all the KP instances in a specific directory with
 * Pisinger Heuristics
 * @version 0.1
 * @date 27/11/19.
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/algorithms/kp_heuristics/Combo.h>
#include <dignea/algorithms/kp_heuristics/ExpKnap.h>
#include <dignea/algorithms/kp_heuristics/MinKnap.h>
#include <dignea/core/AbstractEA.h>
#include <dignea/problems/KPNR.h>
#include <dignea/utilities/printer/JSONPrinter.h>
#include <fmt/core.h>

#include <array>
#include <filesystem>
#include <iostream>
#include <string>
using namespace std;
namespace fs = std::filesystem;
using namespace fmt;

using S = BoolFloatSolution;
using P = Problem<S>;
using EA = AbstractEA<S>;

using runTime = std::array<double, 30>;

std::pair<runTime, runTime> solveInstance(shared_ptr<P> instance) {
    const auto repetitions = 30;
    unique_ptr<EA> minknap = make_unique<MinKnap>();
    unique_ptr<EA> expknap = make_unique<ExpKnap>();

    minknap->setProblem(instance);
    expknap->setProblem(instance);

    runTime minKnapRunTimes;
    runTime expKnapRunTime;

    for (int i = 0; i < repetitions; i++) {
        minknap->run();
        expknap->run();
        auto minTime = minknap->getResults().getSolutions()[0].getFitness();
        auto expTime = expknap->getResults().getSolutions()[0].getFitness();
        minKnapRunTimes[i] = minTime;
        expKnapRunTime[i] = expTime;
    }
    return std::make_pair(minKnapRunTimes, expKnapRunTime);
}

int main(int argc, char **argv) {
    auto nArgs = 2;
    if (argc != nArgs) {
        cerr << "Error in args. <path-to-instances>" << endl;
        exit(EXIT_FAILURE);
    }
    // Parametros del experimento
    string pathToInstance(argv[1]);

    auto entry = fs::path(pathToInstance);
    shared_ptr<P> problem = make_shared<KPNR>(entry);
    std::cout << "Instance: " << entry.filename() << std::endl;
    std::cout << "Number of items: " << problem->getNumberOfVars() << std::endl;

    auto [minTimes, expTimes] = solveInstance(problem);

    string outFile = format("Instance_{}_results", entry.filename().string());
    auto printer = make_unique<JSONPrinter>(outFile);
    json instanceResults;
    instanceResults["minKnap"] = minTimes;
    instanceResults["expTimes"] = expTimes;
    std::cout << instanceResults << std::endl;
    printer->print(outFile, instanceResults);

    return EXIT_SUCCESS;
}
