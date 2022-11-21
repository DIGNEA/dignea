
#include <dignea/algorithms/kp_heuristics/MPW.h>

#include <numeric>

MPW::MPW() : Default() {}

/**
 * @brief Performs the Maximum Profit by Weight Ratio Heuristic for the Knapsack
 * Problem It sorts the elements based on their efficiency and introduces in the
 * solution until the maximum capacity of the knapsack is reached.
 */
void MPW::run() {
    if (this->problem) {
        if (auto pKP = dynamic_pointer_cast<KPNR>(this->problem)) {
            this->startTime = chrono::system_clock::now();
            auto itemsEff = pKP->getItemsSortedByEff();
            auto Q = pKP->getCapacity();
            auto nItems = this->problem->getNumberOfVars();
            auto profits = pKP->getProfits();
            auto weights = pKP->getWeights();
            auto totalProfit = 0;
            auto inKP = 0;

            this->population[0] = BoolFloatSolution(nItems, 1);
            auto vars = this->population[0].getVariables();
            int i = 0;
            while ((i < nItems) && (inKP < Q)) {
                auto idx = itemsEff[i].first;
                if (weights[idx] + inKP <= Q) {
                    vars[idx] = 1;
                    inKP += weights[idx];
                    totalProfit += profits[idx];
                }
                i++;
            }

            this->population[0].setVariables(vars);
            this->population[0].setObjAt(0, totalProfit);
            this->population[0].setFitness(totalProfit);
            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "MPW Heuristic must be used with a KP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in MPW::run");
    }
}
