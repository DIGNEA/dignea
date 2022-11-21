//
// Created by amarrero on 25/3/21.
//

#ifndef DIGNEA_BESTFITNESS_H
#define DIGNEA_BESTFITNESS_H

#include <dignea/utilities/exceptions/NotImplemented.h>

#include "Metric.h"

template <typename S>
class BestFitness : public Metric<float, S> {
   public:
    BestFitness() = default;

    virtual ~BestFitness() = default;

    float computeValue(const Front<S> &front) const override;

    float computeValue(const vector<float> &sols) const override;

    S getBestSolution(const Front<S> &front) const;
};

/**
 * Computes the best fitness in the front
 * @tparam S Type of solution in the Front
 * @param front Front of solutions of type S
 * @return the best fitness found in the front
 */
template <typename S>
float BestFitness<S>::computeValue(const Front<S> &front) const {
    float result = 0;
    if (!front.empty()) {
        for (const S &solution : front.getSolutions()) {
            if (solution.getFitness() > result) {
                result = solution.getFitness();
            }
        }
    }
    return result;
}

/**
 * Computes the best fitness in the front
 * @tparam S Type of solution in the Front
 * @param front Front of solutions of type S
 * @return the best fitness found in the front
 */
template <typename S>
float BestFitness<S>::computeValue(const vector<float> &sols) const {
    float result = 0;
    if (!sols.empty()) {
        result = *max_element(std::begin(sols), std::end(sols));
    }
    return result;
}

/**
 * Calculates the individual with best fitness in the front and returns it
 * @tparam S Type of solution in the Front
 * @param front Front of solutions of type Ft
 * @return the solution of type S with best fitness in the Front
 */
template <typename S>
S BestFitness<S>::getBestSolution(const Front<S> &front) const {
    S bestSolution;
    if (front.empty()) {
        bestSolution.setFitness(0);
    } else {
        float result = 0;
        for (const S &solution : front.getSolutions()) {
            if (solution.getFitness() > result) {
                result = solution.getFitness();
                bestSolution = solution;
            }
        }
    }
    return bestSolution;
}

#endif  // DIGNEA_BESTFITNESS_H
