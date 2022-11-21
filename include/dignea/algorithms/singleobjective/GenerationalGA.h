//
// Created by amarrero on 11/12/20.
//

#ifndef DIGNEA_GENERATIONALGA_H
#define DIGNEA_GENERATIONALGA_H

#include <dignea/core/AbstractGA.h>
#include <dignea/core/Crossover.h>
#include <dignea/core/Mutation.h>
#include <dignea/core/Solution.h>
#include <dignea/replacements/EGenerational.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/random/PseudoRandom.h>

/**
 * @brief Class to represents a Generation Genetic Algorithm (GGA).
 * This algorithms differentiate from others because of using a generational
 * replacement operators.
 * @tparam S Type of individual in the population
 */
template <class S>
class GenerationalGA : public AbstractGA<S> {
   public:
    GenerationalGA();

    virtual ~GenerationalGA() = default;
    /**
     * @brief Get the Name
     *
     * @return string
     */
    string getName() const override { return "Generational Genetic Algorithm"; }
    /**
     * @brief Get the ID of the algorithm
     *
     * @return string
     */
    string getID() const override { return "GGA"; }

   protected:
    void updateProgress() override;
};

/**
 * @brief Creates a RAW instance of a Generational GA algorithm
 * @tparam Problem
 * @tparam S
 */
template <class S>
GenerationalGA<S>::GenerationalGA() : AbstractGA<S>() {
    this->replacement = make_unique<EGenerational<S>>();
}

/**
 * @brief Updates the performed evaluations to the population size on each call.
 * @tparam Problem
 * @tparam S
 */
template <class S>
void GenerationalGA<S>::updateProgress() {
    this->performedEvaluations += this->populationSize;
}

#endif  // DIGNEA_GENERATIONALGA_H
