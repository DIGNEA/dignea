
#include <dignea/algorithms/kp_heuristics/Default.h>

#include <numeric>

Default::Default() : Heuristic<BoolFloatSolution>() {
    this->populationSize = 1;
    this->population.resize(this->populationSize);
}


void Default::run() {
    if (this->problem) {
        if (auto pKP = dynamic_pointer_cast<KPNR>(this->problem)) {
            this->startTime = chrono::system_clock::now();
            auto profits = pKP->getProfits();
            auto weights = pKP->getWeights();
            assert(weights.size() == profits.size());

            auto inKP = 0;
            auto Q = pKP->getCapacity();
            auto totalProfit = 0;
            auto nItems = this->problem->getNumberOfVars();
            this->population[0] = BoolFloatSolution(nItems, 1);
            auto vars = this->population[0].getVariables();
            for (int i = 0; i < nItems; i++) {
                if (inKP + weights[i] <= Q) {
                    vars[i] = 1;
                    inKP += weights[i];
                    totalProfit += profits[i];
                }
            }
            this->population[0].setVariables(vars);
            this->population[0].setObjAt(0, totalProfit);
            this->population[0].setFitness(totalProfit);
            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "Def Heuristic must be used with a KP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in Def::run");
    }
}

void Default::setProblem(shared_ptr<KP> prob) { this->problem = prob; }

void Default::setProblem(KP* prob) { this->problem.reset(prob); }

Front<BoolFloatSolution> Default::getResults() const {
    return Front<BoolFloatSolution>(this->population);
}