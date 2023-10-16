/**
 * @file PisPerformance.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef DIGNEA_PISPERFORMANCE_H
#define DIGNEA_PISPERFORMANCE_H

#include "InstanceFitness.h"

/**
 * @brief Easy Instance fitness computation. This class extends InstanceFitness.
 *
 */
class PisPerformance : public InstanceFitness {
   public:
    PisPerformance() = default;

    virtual ~PisPerformance() = default;
    /**
     * @brief Computes the performance fitness of an instance based on the
     * performance of different solvers. This method computes the fitness as the
     * different between the runtinme of the target solver minus runtime of the
     * others.
     *
     *
     * @param solutions
     * @return float
     */
    float compute(const vector<float> &solutions) const override;
};

#endif  // DIGNEA_PISPERFORMANCE_H
