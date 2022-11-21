//
// Created by amarrero on 1/12/20.
//

#ifndef DIGNEA_SELECTION_H
#define DIGNEA_SELECTION_H

#include <dignea/core/Solution.h>

#include <memory>
#include <random>

using namespace std;

/**
 * @brief  Abstract Selection interface.
 *
 * @tparam S Type of individual in the population
 */
template <class S>
class Selection {
   public:
    Selection() = default;

    virtual ~Selection() = default;

   public:
    /**
     *  @brief Method to perform the selection operation. This is a virtual
     * method that must be implemented in the subclasses. It receives population
     * of individuals and performs the corresponding selection over it.
     * @param  population
     * @return S An individual selected from the population
     */
    virtual S select(const vector<S> &population) = 0;

    /**
     * @brief Method to perform the selection operation. This is a virtual
     * method that must be implemented in the subclasses. It receives population
     * of individuals and a parent number, it performs the corresponding
     * selection over the population and sets the index of the parent in the
     * parent variable.
     *
     * @param population Population of individuals
     * @param parent Index of the parent in the population
     */
    virtual void select(const vector<S> &population, int &parent) = 0;

    virtual std::string getName() const = 0;
};

#endif  // DIGNEA_SELECTION_H
