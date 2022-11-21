//
// Created by amarrero on 25/3/21.
//

#ifndef DIGNEA_WORSTFITNESS_H
#define DIGNEA_WORSTFITNESS_H

#include <dignea/utilities/exceptions/NotImplemented.h>

#include <limits>  // std::numeric_limits

#include "Metric.h"

using namespace std;

template <typename S>
class WorstFitness : public Metric<float, S> {
   public:
    WorstFitness() = default;

    virtual ~WorstFitness() = default;

    float computeValue(const Front<S> &front) const override;

    float computeValue(const vector<float> &sols) const override;

    S getWorstSolution(const Front<S> &front) const;
};

/**
 * Computes and returns the worst fitness in the front
 * @tparam S Type of the solutions in the front
 * @param front Front of solutions of type S
 * @return Worst fitness in front
 */
template <typename S>
float WorstFitness<S>::computeValue(const Front<S> &front) const {
    float result = 0;
    if (!front.empty()) {
        result = numeric_limits<float>::max();
        for (const S &solution : front.getSolutions()) {
            if (solution.getFitness() < result) {
                result = solution.getFitness();
            }
        }
    }
    return result;
}

/**
 * Computes and returns the worst fitness in the front
 * @tparam S Type of the solutions in the front
 * @param front Front of solutions of type S
 * @return Worst fitness in front
 */
template <typename S>
float WorstFitness<S>::computeValue(const vector<float> &sols) const {
    float result = 0;
    if (!sols.empty()) {
        result = *min_element(std::begin(sols), std::end(sols));
    }
    return result;
}

/**
 * Computes and returns the solution with worst fitness in the Front
 * @tparam S  Type of the solution in the front
 * @param front Front of solution of type S
 * @return The solution with the worst fitness
 */
template <typename S>
S WorstFitness<S>::getWorstSolution(const Front<S> &front) const {
    S worstSolution;
    if (front.empty()) {
        throw std::runtime_error("Empty Front in WorstFitness::computeValue");
    }
    float result = numeric_limits<float>::max();
    int idx = 0;
    int dimension = front.getNumOfSolutions();
    vector solutions = front.getSolutions();
    for (int i = 0; i < dimension; i++) {
        if (solutions[i].getFitness() < result) {
            result = solutions[i].getFitness();
            idx = i;
        }
    }
    return solutions[idx];
}

#endif  // DIGNEA_WORSTFITNESS_H
