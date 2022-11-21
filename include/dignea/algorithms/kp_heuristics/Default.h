

#ifndef DIGNEA_KP_DEFAULT_HEURISTIC_H
#define DIGNEA_KP_DEFAULT_HEURISTIC_H

#include <dignea/core/Heuristic.h>
#include <dignea/problems/KPNR.h>

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;
/**
 * @brief Default Heuristic for the Knapsack Problem.
 * Tries to insert each item in the knapsack while there is room for more items.
 *
 */
class Default : public Heuristic<BoolFloatSolution> {
   public:
    Default();

    virtual ~Default() = default;

    void run() override;

    string getName() const { return "Default KP"; };

    string getID() const { return "Def"; }

    void setProblem(shared_ptr<KP> prob);

    void setProblem(KP *prob);

    Front<BoolFloatSolution> getResults() const override;
};

#endif  // DIGNEA_KP_DEFAULT_HEURISTIC_H