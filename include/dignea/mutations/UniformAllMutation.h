/**
 * @file UniformAllMutation.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2019-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_UNIFORMALLMUTATION_H
#define DIGNEA_UNIFORMALLMUTATION_H

#include <dignea/core/Mutation.h>
#include <dignea/utilities/random/PseudoRandom.h>

/**
 * @brief Uniform All Mutation Operator.
 *
 * @tparam S
 */
template <class S>
class UniformAllMutation : public Mutation<S> {
   public:
    UniformAllMutation() = default;

    virtual ~UniformAllMutation() = default;

    void run(S &ind, const double &probability, Problem<S> *) override;

    std::string getName() const override { return "Uniform All Mutation"; };
};

/**
 * @brief Mutates each variable in the individual with a random probability
 * lower than the probability parameter. The new values are always inside the
 * problem bounds.
 *
 * @tparam S
 * @param ind
 * @param probability
 * @param problem
 */
template <class S>
void UniformAllMutation<S>::run(S &ind, const double &probability,
                                Problem<S> *problem) {
    for (int i = 0; i < ind.getNumberOfVars(); i++) {
        if (PseudoRandom::randDouble() < probability) {
            auto var = PseudoRandom::randDouble(problem->getLowerLimit(i),
                                                problem->getUpperLimit(i));
            ind.setVarAt(i, var);
        }
    }
}

#endif  // DIGNEA_UNIFORMALLMUTATION_H