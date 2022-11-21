//
// Created by amarrero on 25/3/21.
//

#ifndef DIGNEA_INSTANCEFITNESS_H
#define DIGNEA_INSTANCEFITNESS_H

#include <dignea/core/Front.h>

#include <memory>

/**
 * @brief Interface to define the fitness calculation of the instances evolved
 * by MEA.
 *
 */
class InstanceFitness {
   public:
    InstanceFitness() = default;
    /**
     * @brief Computes the fitness of an instance evolved by MEA based on the
     * performance of several solvers. This method must be implemented in the
     * subclasses.
     *
     * @param solutions
     * @return float
     */
    virtual float compute(const vector<float> &solutions) const = 0;
};

#endif  // DIGNEA_INSTANCEFITNESS_H
