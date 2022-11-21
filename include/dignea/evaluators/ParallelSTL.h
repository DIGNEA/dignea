//
// Created by amarrero on 23/12/20.
//

#ifndef DIGNEA_PARALLELSTL_H
#define DIGNEA_PARALLELSTL_H

#include <algorithm>
#include <execution>

#include "PopulationEvaluator.h"

template <class S>
class ParallelSTL : public PopulationEvaluator<S> {
   public:
    ParallelSTL() = default;

    virtual ~ParallelSTL() = default;

    void evaluate(vector<S> &population, Problem<S> *problem) override;

    void evaluate(S &solution, Problem<S> *problem) override {
        problem->evaluate(solution);
    };

    string getName() const override;
};

/**
 * Método para evaluar una población de individuos en paralelo usando STL
 * @tparam varsT
 * @tparam objsT
 * @param population
 * @return
 */
template <class S>
void ParallelSTL<S>::evaluate(vector<S> &population, Problem<S> *problem) {
    std::for_each(
        std::execution::par, population.begin(), population.end(),
        [&](unique_ptr<S> &individual) { problem->evaluate(individual); });
}

template <class S>
string ParallelSTL<S>::getName() const {
    return "Parallel STL Population Evaluator";
}

#endif  // DIGNEA_PARALLELSTL_H
