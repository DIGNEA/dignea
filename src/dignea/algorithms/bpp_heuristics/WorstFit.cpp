/**
 * @file WorstFit.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/algorithms/bpp_heuristics/WorstFit.h>

#include <numeric>

WorstFit::WorstFit() : Heuristic<IntIntSolution>() {
    this->populationSize = 1;
    this->population.resize(this->populationSize);
}

void WorstFit::run() {
    if (this->problem) {
        if (auto pBPP = dynamic_pointer_cast<BPP>(this->problem)) {
            this->startTime = chrono::system_clock::now();
            auto items = pBPP->getItems();
            auto binCapacity = pBPP->getCapacity();
            vector<int> binCapacities{};
            vector assignment(pBPP->getNumberOfVars(), 0);

            //  Starts the algorithm
            // It keeps a list of open bins, which is initially empty.

            for (unsigned int i = 0; i < items.size(); i++) {
                // Calculates the bin with maximum load into which the item can
                // fit, if any
                auto bin = -1;
                int minLoad = std::numeric_limits<int>::max();

                for (unsigned int j = 0; j < binCapacities.size(); j++) {
                    // Checks that the item fits
                    if ((items[i] + binCapacities[j] <= binCapacity)) {
                        if (binCapacities[j] < minLoad) {
                            // it is the bin with maximum load found yet
                            minLoad = binCapacities[j];
                            bin = j;
                        }
                    }
                }

                if (bin != -1) {
                    assignment[i] = bin;
                    binCapacities[bin] += items[i];
                } else {
                    // If no such bin is found, open a new bin and put the item
                    // into it
                    assignment[i] = binCapacities.size();
                    binCapacities.push_back(items[i]);
                }
            }

            this->population[0] = IntIntSolution(items.size(), 1);
            this->population[0].setVariables(assignment);
            pBPP->evaluate(this->population[0]);

            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "WorstFit Heuristic must be used with a BPP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in WorstFit::run");
    }
}

void WorstFit::setProblem(shared_ptr<BPP> prob) { this->problem = prob; }

void WorstFit::setProblem(BPP* prob) { this->problem.reset(prob); }

Front<IntIntSolution> WorstFit::getResults() const {
    return Front<IntIntSolution>(this->population);
}