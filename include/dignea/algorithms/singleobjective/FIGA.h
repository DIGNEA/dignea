//
// Created by amarrero on 11/1/21.
//

#ifndef __DIGNEA_FIGA_H__
#define __DIGNEA_FIGA_H__

#include <dignea/core/AbstractGA.h>
#include <dignea/core/Crossover.h>
#include <dignea/core/Mutation.h>
#include <dignea/core/Solution.h>
#include <dignea/replacements/FirstImprove.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/random/PseudoRandom.h>

/**
 * @brief Class to represents a First Improve Genetic Algorithm (FIGA).
 * This algorithms differentiate from others because of using a first improves
 * replacement method. Each new individual replaces its parent if the fitness
 * has improved.
 * @tparam S Type of individual in the population
 */
template <class S>
class FIGA : public AbstractGA<S> {
   public:
    FIGA();

    virtual ~FIGA() = default;

    /**
     * @brief Get the Name
     *
     * @return string
     */
    string getName() const override { return "One-vs-One Genetic Algorithm"; }
    /**
     * @brief Get the ID of the algorithm
     *
     * @return string
     */
    string getID() const override { return "1vs1-GA"; }

   protected:
    void updateProgress() override;
};

/**
 * @brief Creates a RAW FIGA instance
 * @tparam Problem
 * @tparam S
 */
template <class S>
FIGA<S>::FIGA() : AbstractGA<S>() {
    this->replacement = make_unique<FirstImprove<S>>();
}

/**
 * @brief Updates the performed evaluations to the population size on each call.
 * @tparam Problem
 * @tparam S
 */
template <class S>
void FIGA<S>::updateProgress() {
    this->performedEvaluations += this->populationSize;
}

#endif  //__DIGNEA_FIGA_H__
