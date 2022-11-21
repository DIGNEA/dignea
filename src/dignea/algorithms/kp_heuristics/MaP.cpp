
#include <dignea/algorithms/kp_heuristics/MaP.h>

#include <numeric>

MaP::MaP() : Default() {}

/**
 * @brief Performs the Maximum Profit Heuristic for the Knapsack
 * Problem It sorts the elements based on their profit (descending order) and
 * introduces in the solution until the maximum capacity of the knapsack is
 * reached.
 */
void MaP::run() {
    if (this->problem) {
        if (auto pKP = dynamic_pointer_cast<KPNR>(this->problem)) {
            this->startTime = chrono::system_clock::now();

            auto Q = pKP->getCapacity();
            auto nItems = this->problem->getNumberOfVars();
            auto profits = pKP->getProfits();
            auto weights = pKP->getWeights();
            auto totalProfit = 0;
            auto inKP = 0;
            // Needs to sort the variables based on the profit vector
            vector<int> items(nItems);
            std::iota(items.begin(), items.end(), 0);
            sort(items.begin(), items.end(), [&](auto &v1, auto &v2) -> bool {
                return profits[v1] > profits[v2];
            });

            this->population[0] = BoolFloatSolution(nItems, 1);
            auto vars = this->population[0].getVariables();
            int i = 0;
            while ((i < nItems) && (inKP < Q)) {
                auto idx = items[i];
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
                "MaP Heuristic must be used with a KP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in MaP::run");
    }
}
