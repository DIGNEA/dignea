/**
 * @file UniformOneMutation.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2019-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_UNIFORMONEMUTATION_H
#define DIGNEA_UNIFORMONEMUTATION_H

#include <dignea/core/Mutation.h>
#include <dignea/utilities/random/PseudoRandom.h>

/**
 * @brief Uniform One Mutation
 *
 * @tparam S
 */
template <class S>
class UniformOneMutation : public Mutation<S> {
   public:
    UniformOneMutation() = default;

    virtual ~UniformOneMutation() = default;

    void run(S &ind, const double &probability, Problem<S> *) override;

    std::string getName() const override { return "Uniform One Mutation"; };
};

/**
 * @brief Performs the Uniform One Mutation Operator with a probability lower
 * than the given parameter (probability). The operator selects a random gene to
 * mutate and checks that the new value remains in the problem bounds.
 *
 * @tparam S
 * @param ind
 * @param probability
 * @param problem
 */
template <class S>
void UniformOneMutation<S>::run(S &ind, const double &probability,
                                Problem<S> *problem) {
    if (PseudoRandom::randDouble() < probability) {
        int varIndex = PseudoRandom::randInt(0, ind.getNumberOfVars() - 1);
        auto varNewValue = PseudoRandom::randDouble(
            problem->getLowerLimit(varIndex), problem->getUpperLimit(varIndex));
        ind.setVarAt(varIndex, varNewValue);
    }
}

#endif  // DIGNEA_UNIFORMONEMUTATION_H