//
// Created by amarrero on 25/3/21.
//

#ifndef DIGNEA_AVERAGEFITNESS_H
#define DIGNEA_AVERAGEFITNESS_H

#include "Metric.h"

template <typename S>
class AverageFitness : public Metric<float, S> {
   public:
    AverageFitness() = default;

    virtual ~AverageFitness() = default;

    float computeValue(const Front<S> &front) const override;

    float computeValue(const vector<float> &sols) const override;
};

/**
 * Computes the average value of the fitness for each solution in the Front
 * @tparam S
 * @param front
 * @return
 */
template <typename S>
float AverageFitness<S>::computeValue(const Front<S> &front) const {
    if (front.empty()) {
        return 0.0;
    }
    float result = 0;
    for (const S &solution : front.getSolutions()) {
        result += solution.getFitness();
    }
    result = result / front.getNumOfSolutions();
    return result;
}

/**
 * Computes the average value of the fitness for each solution in the Front
 * @tparam S
 * @param front
 * @return
 */
template <typename S>
float AverageFitness<S>::computeValue(const vector<float> &sols) const {
    if (sols.empty()) {
        return 0.0;
    }
    return std::accumulate(sols.begin(), sols.end(), 0.0) / sols.size();
}

#endif  // DIGNEA_AVERAGEFITNESS_H
