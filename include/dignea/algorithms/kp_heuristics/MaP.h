

#ifndef DIGNEA_KP_MAP_HEURISTIC_H
#define DIGNEA_KP_MAP_HEURISTIC_H

#include <dignea/algorithms/kp_heuristics/Default.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

/**
 * @brief Maximum Profit Heuristic.
 *
 * This class performs the Maximum Profit Heuristic for the Knapsack
 * Problem. It sorts the elements based on their profit (descending order) and
 * introduces in the solution until the maximum capacity of the knapsack is
 * reached.
 *
 */
class MaP : public Default {
   public:
    MaP();

    virtual ~MaP() = default;

    void run() override;

    string getName() const { return "MaP KP"; };

    string getID() const { return "MaP"; }
};

#endif  // DIGNEA_KP_MAP_HEURISTIC_H