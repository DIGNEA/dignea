/**
 * @file ExpKnap.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DIGNEA_KP_ExpKnap_HEURISTIC_H
#define DIGNEA_KP_ExpKnap_HEURISTIC_H

#include <dignea/core/Heuristic.h>
#include <dignea/problems/KPNR.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;
/**
 * @brief ExpKnap Heuristic for the Knapsack Problem.
 * Tries to insert each item in the knapsack while there is room for more items.
 *
 */
class ExpKnap : public Heuristic<BoolFloatSolution> {
   public:
    ExpKnap();

    virtual ~ExpKnap() = default;

    void run() override;

    string getName() const { return "ExpKnap KP"; };

    string getID() const { return "ExpKnap"; }

    void setProblem(shared_ptr<KP> prob);

    void setProblem(KP *prob);

    Front<BoolFloatSolution> getResults() const override;
};

#endif  // DIGNEA_KP_ExpKnap_HEURISTIC_H