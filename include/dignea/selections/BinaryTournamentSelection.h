/**
 * @file BinaryTournamentSelection.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2020-12-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_BINARYTOURNAMENTSELECTION_H
#define DIGNEA_BINARYTOURNAMENTSELECTION_H

#include <dignea/core/Selection.h>
#include <dignea/utilities/random/PseudoRandom.h>

/**
 * @brief Binary Tournament Selection Operator.
 *
 * @tparam S
 */
template <class S>
class BinaryTournamentSelection : public Selection<S> {
   public:
    BinaryTournamentSelection() = default;

    virtual ~BinaryTournamentSelection() = default;

    void select(const vector<S> &, int &) override;

    S select(const vector<S> &population) override;

    std::string getName() const override {
        return "Binary Tournament Selection";
    };
};

/**
 * @brief Performs the Binary Tournament Selection operator over the population
 * and returns the selected parent.
 *
 * @tparam S
 * @param population
 * @return S
 */
template <class S>
S BinaryTournamentSelection<S>::select(const vector<S> &population) {
    int popSize = population.size() - 1;
    int firstOption = PseudoRandom::randInt(0, popSize);
    int secondOption = PseudoRandom::randInt(0, popSize);
    int parent1 = min(firstOption, secondOption);
    S result = population[parent1];
    return result;
}

/**
 * @brief Performs the Binary Tournament Selection operator over the population
 * and sets the index of the parent inside the population to the parent
 * parameter.
 *
 * @tparam S
 * @param population
 * @param parent
 */
template <class S>
void BinaryTournamentSelection<S>::select(const vector<S> &population,
                                          int &parent) {
    int popSize = population.size() - 1;
    int firstOption = PseudoRandom::randInt(0, popSize);
    int secondOption = PseudoRandom::randInt(0, popSize);
    parent = min(firstOption, secondOption);
}

#endif  // DIGNEA_BINARYTOURNAMENTSELECTION_H
