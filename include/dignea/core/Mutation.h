//
// Created by amarrero on 17/10/19.
//

#ifndef DIGNEA_MUTATION_H
#define DIGNEA_MUTATION_H

#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>

#include <memory>

using namespace std;

/**
 * @brief Abstract Mutation interface.
 *
 * @tparam S Type of individual in the population
 */
template <class S>
class Mutation {
   public:
    Mutation() = default;

    virtual ~Mutation() = default;

   public:
    /**
     *  @brief Method to perform the mutation operation. This is a virtual
     * method that must be implemented in the subclasses. It receives an
     * individual and performs the corresponding mutation over it.
     *
     * @param ind
     * @param probability
     * @param Problem pointer
     */
    virtual void run(S &ind, const double &probability, Problem<S> *) = 0;

    virtual std::string getName() const = 0;
};

#endif  // DIGNEA_MUTATION_H