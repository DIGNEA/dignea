/**
 * @file NextFit.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-01
 *
 * Next Fit Heuristic for the BPP
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DIGNEA_BPP_NEXT_FIT_H
#define DIGNEA_BPP_NEXT_FIT_H

#include <dignea/core/Heuristic.h>
#include <dignea/problems/BPP.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

/**
 * @brief Next Fit Heuristic for the BPP.
 * Places each item into the current bin
 *
 */
class NextFit : public Heuristic<IntIntSolution> {
   public:
    NextFit();

    virtual ~NextFit() = default;

    void run() override;

    string getName() const { return "NextFit"; };

    string getID() const { return "NextFit"; }

    void setProblem(shared_ptr<BPP> prob);

    void setProblem(BPP *prob);

    Front<IntIntSolution> getResults() const override;
};

#endif  // DIGNEA_BPP_NEXT_FIT_H