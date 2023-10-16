/**
 * @file FirstFit.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/algorithms/bpp_heuristics/FirstFit.h>
#include <fmt/ranges.h>

#include <numeric>

FirstFit::FirstFit() : Heuristic<IntIntSolution>() {
    this->populationSize = 1;
    this->population.resize(this->populationSize);
}

void FirstFit::run() {
    if (this->problem) {
        if (auto pBPP = dynamic_pointer_cast<BPP>(this->problem)) {
            this->startTime = chrono::system_clock::now();
            auto items = pBPP->getItems();
            auto binCapacity = pBPP->getCapacity();
            vector<int> openBins{};
            vector assignment(pBPP->getNumberOfVars(), 0);

            //  Starts the algorithm
            for (unsigned int i = 0; i < items.size(); i++) {
                bool placed = false;
                for (unsigned int k = 0; k < openBins.size(); k++) {
                    if (openBins[k] + items[i] <= binCapacity) {
                        openBins[k] += items[i];
                        assignment[i] = k;
                        placed = true;
                        break;
                    }
                }
                if (!placed) {
                    // We open a new bin and the coming items is placed inside
                    // it
                    openBins.push_back(items[i]);
                    assignment[i] = openBins.size() - 1;
                }
            }

            this->population[0] = IntIntSolution(items.size(), 1);
            this->population[0].setVariables(assignment);
            pBPP->evaluate(this->population[0]);

            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "FirstFit Heuristic must be used with a BPP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in FirstFit::run");
    }
}

void FirstFit::setProblem(shared_ptr<BPP> prob) { this->problem = prob; }

void FirstFit::setProblem(BPP* prob) { this->problem.reset(prob); }

Front<IntIntSolution> FirstFit::getResults() const {
    return Front<IntIntSolution>(this->population);
}