

#ifndef DIGNEA_TSP_NearestNeighbour_HEURISTIC_H
#define DIGNEA_TSP_NearestNeighbour_HEURISTIC_H

#include <dignea/core/Heuristic.h>
#include <dignea/problems/TSP.h>

#include <nlohmann/json.hpp>
#include <vector>

namespace tsp_heuristics {

using json = nlohmann::json;
using namespace std;
/**
 * @brief NearestNeighbour Heuristic for the Knapsack Problem.
 * Tries to insert each item in the knapsack while there is room for more items.
 *
 */
class NearestNeighbour : public Heuristic<IntFloatSolution> {
   public:
    NearestNeighbour();

    virtual ~NearestNeighbour() = default;

    void run() override;

    string getName() const { return "NearestNeighbour TSP"; };

    string getID() const { return "NN TSP"; }

    void setProblem(shared_ptr<TSP> prob);

    void setProblem(TSP *prob);

    Front<IntFloatSolution> getResults() const override;
};

}  // namespace tsp_heuristics
#endif  // DIGNEA_TSP_NearestNeighbour_HEURISTIC_H