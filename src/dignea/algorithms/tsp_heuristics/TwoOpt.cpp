
#include <dignea/algorithms/tsp_heuristics/NearestNeighbour.h>
#include <dignea/algorithms/tsp_heuristics/TwoOpt.h>

#include <algorithm>
#include <numeric>
#include <ranges>
#include <set>
#include <tuple>

using namespace tsp_heuristics;

TwoOpt::TwoOpt(const bool randomInit)
    : Heuristic<IntFloatSolution>(), randomInitialisation(randomInit) {
    this->populationSize = 1;
    this->population.resize(this->populationSize);
}

/**
 * @brief The TwoOpt heuristic gradually constructs a tour by
 * repeatedly selecting the shortest edge and adding it to the tour as long as
 * it doesn’t create a cycle with less than N edges, or increases the degree of
 * any node to more than 2. We must not add the same edge twice of course.
 *
 */
void TwoOpt::run() {
    if (this->problem) {
        if (auto pTSP = dynamic_pointer_cast<TSP>(this->problem)) {
            this->startTime = chrono::system_clock::now();
            if (this->randomInitialisation) {
                // Creates a random solution to start
                this->population[0] = pTSP->createSolution();
            } else {
                auto nn = make_unique<NearestNeighbour>();
                nn->setProblem(pTSP);
                nn->run();
                this->population[0] = nn->getPopulation()[0];
            }
            pTSP->evaluate(this->population[0]);

            auto nCities = this->problem->getNumberOfVars();

            auto improve = 0;
            const int MAX_IMPROVE = 10;
            while (improve < MAX_IMPROVE) {
                auto currentBest = this->population[0].getObjAt(0);
                auto vars = this->population[0].getVariables();
                for (int i = 1; i < nCities - 1; i++) {
                    for (int k = i + 1; k < nCities; k++) {
                        auto newTour = performSwap(i, k, vars);
                        auto newSolution = pTSP->createSolution();
                        newSolution.setVariables(newTour);
                        pTSP->evaluate(newSolution);

                        if (newSolution.getObjAt(0) < currentBest) {
                            improve = 0;
                            currentBest = newSolution.getObjAt(0);
                            vars = newSolution.getVariables();
                            this->population[0] = newSolution;
                        }
                    }
                }
                improve++;
            }
            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "TwoOpt Heuristic must be used with a TSP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in TwoOpt::run");
    }
}

/**
 * @brief Applies the 2-Opt heuristic swap on an existing tour. It reverses the
 * cities between i and k.
 *
 * @param i
 * @param k
 * @param tour
 * @return vector<int>
 */
vector<int> TwoOpt::performSwap(const int& i, const int& k,
                                const vector<int>& tour) {
    vector newTour = tour;

    auto dec = 0;
    for (int idx = i; idx <= k; idx++) {
        newTour[idx] = tour[(k - dec)];
        dec++;
    }
    return newTour;
}

void TwoOpt::setProblem(shared_ptr<TSP> prob) { this->problem = prob; }

void TwoOpt::setProblem(TSP* prob) { this->problem.reset(prob); }

Front<IntFloatSolution> TwoOpt::getResults() const {
    return Front<IntFloatSolution>(this->population);
}