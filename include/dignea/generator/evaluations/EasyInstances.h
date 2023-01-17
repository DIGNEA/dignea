//
// Created by amarrero on 25/3/21.
//

#ifndef DIGNEA_EASYINSTANCES_H
#define DIGNEA_EASYINSTANCES_H

#include "InstanceFitness.h"

/**
 * @brief Easy Instance fitness computation. This class extends InstanceFitness.
 *
 */
class EasyInstances : public InstanceFitness {
   public:
    EasyInstances() = default;

    virtual ~EasyInstances() = default;
    /**
     * @brief Computes the performance fitness of an instance based on the
     * performance of different solvers. This method computes the fitness as the
     * different between the performance of the target solver minus the maximum
     * performance of the others.
     *
     * solutions[0] - max(solutions[1..])
     *
     * @param solutions
     * @return float
     */
    float compute(const vector<float> &solutions) const override;
};

#endif  // DIGNEA_EASYINSTANCES_H
