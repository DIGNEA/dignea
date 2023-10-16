
#include <dignea/algorithms/tsp_heuristics/NearestNeighbour.h>

#include <numeric>
#include <set>
using namespace tsp_heuristics;

NearestNeighbour::NearestNeighbour() : Heuristic<IntFloatSolution>() {
    this->populationSize = 1;
    this->population.resize(this->populationSize);
}

void NearestNeighbour::run() {
    if (this->problem) {
        if (auto pTSP = dynamic_pointer_cast<TSP>(this->problem)) {
            this->startTime = chrono::system_clock::now();

            auto length = 0;
            auto nCities = this->problem->getNumberOfVars();
            auto distances = pTSP->getDistances();

            this->population[0] = pTSP->createSolution();
            auto vars = this->population[0].getVariables();
            unsigned int currentCity = 0;
            std::set<unsigned> citiesInTour{currentCity};
            vars[0] = currentCity;
            auto idx = 1;
            while (citiesInTour.size() != (unsigned)nCities) {
                auto nextCity = 0;
                auto minDist = std::numeric_limits<float>::max();
                for (int j = 0; j < nCities; j++) {
                    if (distances[currentCity][j] < minDist &&
                        !citiesInTour.contains(j)) {
                        minDist = distances[currentCity][j];
                        nextCity = j;
                    }
                }
                citiesInTour.insert(nextCity);
                vars[idx] = nextCity;
                idx++;
                length += minDist;
            }
            // Completing the cycle
            vars[nCities] = 0;
            length += distances[currentCity][0];

            this->population[0].setVariables(vars);
            this->population[0].setObjAt(0, length);
            this->population[0].setFitness(1.0 / length);
            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "NN Heuristic must be used with a TSP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in NN::run");
    }
}

void NearestNeighbour::setProblem(shared_ptr<TSP> prob) {
    this->problem = prob;
}

void NearestNeighbour::setProblem(TSP* prob) { this->problem.reset(prob); }

Front<IntFloatSolution> NearestNeighbour::getResults() const {
    return Front<IntFloatSolution>(this->population);
}