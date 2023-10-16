/**
 * @file NextFit.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/algorithms/bpp_heuristics/NextFit.h>

#include <numeric>

NextFit::NextFit() : Heuristic<IntIntSolution>() {
    this->populationSize = 1;
    this->population.resize(this->populationSize);
}

void NextFit::run() {
    if (this->problem) {
        if (auto pBPP = dynamic_pointer_cast<BPP>(this->problem)) {
            this->startTime = chrono::system_clock::now();
            auto items = pBPP->getItems();
            auto binCapacity = pBPP->getCapacity();
            vector assignment(pBPP->getNumberOfVars(), 0);

            //  Starts the algorithm
            auto binCount = 0;
            auto remainCapacity = binCapacity;

            for (unsigned int i = 0; i < items.size(); i++) {
                auto item = items[i];
                // If it fits in the current bin
                // we include it
                if (item <= remainCapacity) {
                    remainCapacity -= item;
                } else {
                    // Otherwise we open a new bin
                    binCount++;
                    remainCapacity = binCapacity - item;
                }
                // Anyways we assign the item to the current bin counter
                assignment[i] = binCount;
            }

            this->population[0] = IntIntSolution(items.size(), 1);
            this->population[0].setVariables(assignment);
            pBPP->evaluate(this->population[0]);

            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "NextFit Heuristic must be used with a BPP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in NextFit::run");
    }
}

void NextFit::setProblem(shared_ptr<BPP> prob) { this->problem = prob; }

void NextFit::setProblem(BPP* prob) { this->problem.reset(prob); }

Front<IntIntSolution> NextFit::getResults() const {
    return Front<IntIntSolution>(this->population);
}