
#include <dignea/algorithms/tsp_heuristics/Greedy.h>

#include <numeric>
#include <ranges>
#include <set>
#include <tuple>

using namespace tsp_heuristics;

Greedy::Greedy() : Heuristic<IntFloatSolution>() {
    this->populationSize = 1;
    this->population.resize(this->populationSize);
}

/**
 * @brief The Greedy heuristic gradually constructs a tour by
 * repeatedly selecting the shortest edge and adding it to the tour as long as
 * it doesn’t create a cycle with less than N edges, or increases the degree of
 * any node to more than 2. We must not add the same edge twice of course.
 *
 */
void Greedy::run() {
    if (this->problem) {
        if (auto pTSP = dynamic_pointer_cast<TSP>(this->problem)) {
            this->startTime = chrono::system_clock::now();

            auto length = 0;
            auto nCities = this->problem->getNumberOfVars();
            auto distances = pTSP->getDistances();

            std::set<std::tuple<int, int, float>> edgesTour;
            vector<int> order(nCities, 0);
            vector<std::tuple<int, int, float>> edges;

            // Sorting the edges by minimum distance
            for (int i = 0; i < nCities; i++) {
                for (int j = i + 1; j < nCities; j++) {
                    edges.push_back(std::make_tuple(i, j, distances[i][j]));
                }
            }
            std::sort(edges.begin(), edges.end(), [](auto &tup1, auto &tup2) {
                return std::get<2>(tup1) < std::get<2>(tup2);
            });

            for (auto &e : edges) {
                auto [i, j, dist] = e;
                auto reverseEdge = std::make_tuple(j, i, dist);
                if (edgesTour.contains(e) || edgesTour.contains(reverseEdge))
                    continue;
                if (order[i] >= 2 || order[j] >= 2) continue;
                // Lo incluimos en la solucion
                edgesTour.insert(e);
                order[i]++;
                order[j]++;
                length += dist;
                if ((int)edgesTour.size() == nCities) break;
            }

            this->population[0] = pTSP->createSolution();
            auto vars = this->population[0].getVariables();
            auto currentCity = 0;
            // Start and end of trip
            vars[0] = currentCity;
            vars[nCities - 1] = currentCity;
            this->population[0].setVariables(vars);
            this->population[0].setObjAt(0, length);
            this->population[0].setFitness(1.0 / length);
            this->endTime = chrono::system_clock::now();
            this->finishProgress();
        } else {
            throw std::runtime_error(
                "Greedy Heuristic must be used with a TSP instance");
        }
    } else {
        throw std::runtime_error("Problem is not set in Greedy::run");
    }
}

void Greedy::setProblem(shared_ptr<TSP> prob) { this->problem = prob; }

void Greedy::setProblem(TSP *prob) { this->problem.reset(prob); }

Front<IntFloatSolution> Greedy::getResults() const {
    return Front<IntFloatSolution>(this->population);
}