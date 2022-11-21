/**
 * @file ScrambleMutation.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_SCRAMBLEMUTATION_H
#define DIGNEA_SCRAMBLEMUTATION_H

#include <dignea/core/Mutation.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <algorithm>
#include <random>
/**
 * @brief Scramble Mutation Operator for Permutation Based Problems like TSP.
 * Fixes the first and last genes to zero (TSP constraint).
 *
 * @tparam S
 */
template <class S>
class ScrambleMutation : public Mutation<S> {
   public:
    ScrambleMutation() = default;

    virtual ~ScrambleMutation() = default;

    void run(S &ind, const double &probability, Problem<S> *) override;

    std::string getName() const override { return "Scramble Mutation"; };
};

/**
 * @brief Performs Scramble Mutation Operation
 *
 * Two positions (genes) in the chromosome are selected at random and all the
 * genes between the points are shuffled randomly
 *
 * @tparam S
 * @param ind
 * @param probability
 * @param problem
 */
template <class S>
void ScrambleMutation<S>::run(S &ind, const double &probability,
                              Problem<S> *problem) {
    int start = 0;
    int end = 0;
    while ((start == end)) {
        // Cannot change first and last element
        // -2 to skip the last element too
        start = PseudoRandom::randInt(1, ind.getNumberOfVars() - 2);
        end = PseudoRandom::randInt(1, ind.getNumberOfVars() - 2);
    }
    try {
        std::random_device rd;
        std::default_random_engine gen(rd());
        vector vars = ind.getVariables();
        std::shuffle(vars.begin() + start, vars.begin() + end, gen);
        ind.setVariables(vars);
    } catch (OutOfRange const &err) {
        std::string msg =
            std::string(err.what()) +
            " from Scramble operator with start: " + to_string(start).c_str() +
            " and end: " + to_string(end).c_str() +
            " with ind.vars().size() = " + to_string(ind.getNumberOfVars());
        throw OutOfRange(msg);
    }
}

#endif  // DIGNEA_Scramble_H