/**
 * @file SwapMutation.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_SWAPMUTATION_H
#define DIGNEA_SWAPMUTATION_H

#include <dignea/core/Mutation.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Swap Mutation Operator for Permutation Based Problems like TSP.
 * Fixes the first and last genes to zero (TSP constraint).
 *
 * @tparam S
 */
template <class S>
class SwapMutation : public Mutation<S> {
   public:
    SwapMutation() = default;

    virtual ~SwapMutation() = default;

    void run(S &ind, const double &probability, Problem<S> *) override;

    std::string getName() const override { return "Swap Mutation"; };
};

/**
 * @brief Performs Swap Mutation Operation
 *
 * Two positions (genes) in the chromosome are selected at random and their
 * allele values swapped.
 *
 * @tparam S
 * @param ind
 * @param probability
 * @param problem
 */
template <class S>
void SwapMutation<S>::run(S &ind, const double &probability,
                          Problem<S> *problem) {
    int idx2Swap = 0;
    int idx2Swap2 = 0;
    while ((idx2Swap == idx2Swap2)) {
        // Cannot change first and last element
        // -2 to skip the last element too
        idx2Swap = PseudoRandom::randInt(1, ind.getNumberOfVars() - 2);
        idx2Swap2 = PseudoRandom::randInt(1, ind.getNumberOfVars() - 2);
    }
    try {
        auto iGene = ind.getVarAt(idx2Swap);
        auto iGene2 = ind.getVarAt(idx2Swap2);
        ind.setVarAt(idx2Swap, iGene2);
        ind.setVarAt(idx2Swap2, iGene);
    } catch (OutOfRange const &err) {
        auto j = ind.to_json();
        std::string msg = std::string(err.what()) +
                          " from SwapMutation operator with idx1: " +
                          to_string(idx2Swap).c_str() +
                          " and idx2: " + to_string(idx2Swap2).c_str() +
                          " with ind.getNumberOfVars() = " +
                          to_string(ind.getNumberOfVars()) +
                          "\n Individual is: \n" + j.dump(4);
        std::cout << msg << std::endl;
        exit(1);
    }
}

#endif  // DIGNEA_SWAPMUTATION_H