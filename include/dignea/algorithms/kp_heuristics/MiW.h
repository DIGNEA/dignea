

#ifndef DIGNEA_KP_MIW_HEURISTIC_H
#define DIGNEA_KP_MIW_HEURISTIC_H

#include <dignea/algorithms/kp_heuristics/Default.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

/**
 * @brief Performs the Minimum Weight Heuristic for the Knapsack Problem
 * It sorts the elements based on their weights and introduces in the solution
 * until the maximum capacity of the knapsack is reached.
 *
 */
class MiW : public Default {
   public:
    MiW();

    virtual ~MiW() = default;

    void run() override;

    string getName() const { return "MiW KP"; };

    string getID() const { return "MiW"; }
};

#endif  // DIGNEA_KP_MIW_HEURISTIC_H