

#ifndef DIGNEA_KP_MPW_HEURISTIC_H
#define DIGNEA_KP_MPW_HEURISTIC_H

#include <dignea/algorithms/kp_heuristics/Default.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

/**
 * @brief Performs the Maximum Profit by Weight Ratio Heuristic for the Knapsack
 * Problem It sorts the elements based on their efficiency and introduces in the
 * solution until the maximum capacity of the knapsack is reached.
 *
 */
class MPW : public Default {
   public:
    MPW();

    virtual ~MPW() = default;

    void run() override;

    string getName() const { return "MPW KP"; };

    string getID() const { return "MPW"; }
};

#endif  // DIGNEA_KP_MPW_HEURISTIC_H