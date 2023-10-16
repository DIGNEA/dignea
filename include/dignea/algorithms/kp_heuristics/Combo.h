/**
 * @file Combo.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-04-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef DIGNEA_KP_Combo_HEURISTIC_H
#define DIGNEA_KP_Combo_HEURISTIC_H

#include <dignea/core/Heuristic.h>
#include <dignea/problems/KPNR.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

/**
 * @brief Combo Heuristic for the Knapsack Problem.
 *
 *
 */
class Combo : public Heuristic<BoolFloatSolution> {
   public:
    Combo();

    virtual ~Combo() = default;

    void run() override;

    string getName() const { return "Combo KP"; };

    string getID() const { return "Combo"; }

    void setProblem(shared_ptr<KP> prob);

    void setProblem(KP *prob);

    Front<BoolFloatSolution> getResults() const override;
};

#endif  // DIGNEA_KP_Combo_HEURISTIC_H