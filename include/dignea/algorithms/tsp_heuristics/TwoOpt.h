

#ifndef DIGNEA_TSP_TwoOpt_HEURISTIC_H
#define DIGNEA_TSP_TwoOpt_HEURISTIC_H

#include <dignea/core/Heuristic.h>
#include <dignea/problems/TSP.h>

#include <nlohmann/json.hpp>
#include <vector>

namespace tsp_heuristics {

using json = nlohmann::json;
using namespace std;
/**
 * @brief TwoOpt Heuristic for the Knapsack Problem.
 * Tries to insert each item in the knapsack while there is room for more items.
 *
 */
class TwoOpt : public Heuristic<IntFloatSolution> {
   public:
    TwoOpt(const bool randomInit = true);

    virtual ~TwoOpt() = default;

    void run() override;

    string getName() const {
        return randomInitialisation ? "TwoOpt TSP" : "NN-TwoOpt TSP";
    };

    string getID() const {
        return randomInitialisation ? "TwoOpt TSP" : "NN-TwoOpt TSP";
    }

    void setProblem(shared_ptr<TSP> prob);

    void setProblem(TSP* prob);

    Front<IntFloatSolution> getResults() const override;

   private:
    vector<int> performSwap(const int& i, const int& k,
                            const vector<int>& tour);

    float calculateLength(const vector<int>& tour, const TSP* problem);

   private:
    bool randomInitialisation;
};

}  // namespace tsp_heuristics
#endif  // DIGNEA_TSP_TwoOpt_HEURISTIC_H