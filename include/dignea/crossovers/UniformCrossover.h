/**
 * @file UniformCrossover.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2019-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_UNIFORMCROSSOVER_H
#define DIGNEA_UNIFORMCROSSOVER_H

#include <dignea/core/Crossover.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <random>

/**
 * @brief Uniform Crossover Operator
 *
 * @tparam S
 */
template <class S>
class UniformCrossover : public Crossover<S> {
   public:
    UniformCrossover() = default;

    virtual ~UniformCrossover() = default;

    void run(S &firstInd, S &secondInd) override;

    std::string getName() const override { return "Uniform One Crossover"; };
};

/**
 * @brief Performs the Uniform Crossover between the two individuals.
 *
 * @tparam S
 * @param firstInd
 * @param secondInd
 */
template <class S>
void UniformCrossover<S>::run(S &firstInd, S &secondInd) {
    // Usamos C++17 para no declarar el tipo del vector que obtenemos
    vector secondIndVars = secondInd.getVariables();
    vector firstIndVars = firstInd.getVariables();

    for (int i = 0; i < firstInd.getNumberOfVars(); i++) {
        if (PseudoRandom::randDouble() < 0.5) {
            auto tmpVariable = secondIndVars[i];
            auto copyVar = firstIndVars[i];
            secondIndVars[i] = copyVar;
            firstIndVars[i] = tmpVariable;
        }
    }
    firstInd.setVariables(firstIndVars);
    secondInd.setVariables(secondIndVars);
}

#endif  // DIGNEA_UNIFORMCROSSOVER_H