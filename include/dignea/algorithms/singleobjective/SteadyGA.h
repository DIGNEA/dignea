//
// Created by amarrero on 19/10/19.
//

#ifndef DIGNEA_STEADYGA_H
#define DIGNEA_STEADYGA_H

#include <dignea/core/AbstractGA.h>
#include <dignea/core/Solution.h>
#include <dignea/replacements/ReplaceWorst.h>
#include <dignea/utilities/Comparator.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/random/PseudoRandom.h>

/**
 * @brief Class to represents a Steady State Genetic Algorithm (SSGA)
 * At each generation, a new individual is included in the population.
 * @tparam S Type of individual in the population
 */
template <class S>
class SteadyGA : public AbstractGA<S> {
   public:
    SteadyGA();

    virtual ~SteadyGA() = default;
    /**
     * @brief Get the Name
     *
     * @return string
     */
    string getName() const override { return "Steady-State Genetic Algorithm"; }
    /**
     * @brief Get the ID of the algorithm
     *
     * @return string
     */
    string getID() const override { return "SSGA"; }

   protected:
    vector<S> createMating() override;

    void updateProgress() override;
};

/**
 * @brief Creates a RAW Steady GA instance
 * @tparam Problem
 * @tparam S
 */
template <class S>
SteadyGA<S>::SteadyGA() : AbstractGA<S>() {
    this->replacement = make_unique<ReplaceWorst<S>>();
}

/**
 * @brief Creates the mating of individuals to be evaluated
 * @tparam Problem
 * @tparam S
 */
template <class S>
vector<S> SteadyGA<S>::createMating() {
    vector<S> offspring(1);
    S child1 = this->selection->select(this->population);
    S child2 = this->selection->select(this->population);
    // Aplicamos los operadores geneticos
    this->reproduction(child1, child2);
    offspring[0] = child1;
    return offspring;
}

/**
 * @brief Updates the performed evaluations by one on each call.
 * @tparam Problem
 * @tparam S
 */
template <class S>
void SteadyGA<S>::updateProgress() {
    this->performedEvaluations++;
}

#endif  // DIGNEA_STEADYGA_H
