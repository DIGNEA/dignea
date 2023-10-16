
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
#include <dignea/algorithms/tsp_heuristics/Greedy.h>
#include <dignea/algorithms/tsp_heuristics/NearestNeighbour.h>
#include <dignea/algorithms/tsp_heuristics/TwoOpt.h>
#include <dignea/core/Front.h>
#include <dignea/core/Heuristic.h>
#include <dignea/problems/TSP.h>
#include <dignea/utilities/printer/JSONPrinter.h>
#include <fmt/core.h>

#include <array>
#include <filesystem>
#include <string>

using namespace fmt;
using namespace std;
namespace fs = std::filesystem;
using namespace tsp_heuristics;
using EA = Heuristic<IntFloatSolution>;

int main(int argc, char** argv) {
    auto nArgs = 2;
    if (argc != nArgs) {
        cerr << "Error in args. <path-to-instance>" << endl;
        exit(EXIT_FAILURE);
    }

    string pathToInstance = (argv[1]);
    vector<unique_ptr<EA>> algorithms;
    algorithms.push_back(make_unique<NearestNeighbour>());
    algorithms.push_back(make_unique<Greedy>());
    algorithms.push_back(make_unique<TwoOpt>());
    algorithms.push_back(make_unique<TwoOpt>(false));

    /*  vector<unique_ptr<EA>> algorithms = {make_unique<NearestNeighbour>(),
                                          make_unique<Greedy>(),
                                          make_unique<TwoOpt>()}; */

    auto tsp = make_shared<TSP>(pathToInstance);
    auto tspName = fs::path(pathToInstance).filename().string();

    for (unique_ptr<EA>& alg : algorithms) {
        auto outputFile =
            format("TSP_instance_{}_solved_with_{}", tspName, alg->getName());

        alg->setProblem(tsp);
        alg->run();
        auto results = alg->getResults();
        json data = results.to_json();
        auto printer = make_unique<JSONPrinter>("printer");
        printer->print(outputFile, data);
        std::cout << alg->getName() << " got a solution with distance: "
                  << results.getSolutions()[0].getObjAt(0) << std::endl;
    }

    std::cout << "Experiment done" << std::endl;
    return EXIT_SUCCESS;
}
