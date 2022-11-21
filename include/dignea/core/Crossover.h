//
// Created by amarrero on 17/10/19.
//

#ifndef DIGNEA_CROSSOVER_H
#define DIGNEA_CROSSOVER_H

#include <dignea/core/Solution.h>

#include <memory>

using namespace std;

/**
 * @brief Abstract Crossover interface.
 * @tparam S Type of individual in the population
 */
template <class S>
class Crossover {
   public:
    Crossover() = default;

    virtual ~Crossover() = default;

   public:
    /**
     * @brief Method to perform the crossover operation. This is a virtual
     * method that must be implemented in the subclasses. It receives two
     * individuals and performs the corresponding crossover between them.
     *
     * @param firstInd
     * @param secondInd
     */
    virtual void run(S &firstInd, S &secondInd) = 0;

    virtual std::string getName() const = 0;
};

#endif  // DIGNEA_CROSSOVER_H
