//
// Created by amarrero on 2/12/20.
//

#ifndef DIGNEA_PARALLELOMPEVALUATOR_H
#define DIGNEA_PARALLELOMPEVALUATOR_H

#include <iostream>

#include "PopulationEvaluator.h"
#include "omp.h"

template <class S>
class ParallelOMPEvaluator : public PopulationEvaluator<S> {
   public:
    ParallelOMPEvaluator();

    explicit ParallelOMPEvaluator(const int &nThreads);

    virtual ~ParallelOMPEvaluator() = default;

    void evaluate(vector<S> &population, Problem<S> *problem) override;

    void evaluate(S &solution, Problem<S> *problem) override {
        problem->evaluate(solution);
    };

    std::string getName() const override {
        return "Parallel OMP Population Evaluator";
    }

   private:
    int numberOfThreads;
};

template <class S>
ParallelOMPEvaluator<S>::ParallelOMPEvaluator() {
    numberOfThreads = 4;
}

template <class S>
ParallelOMPEvaluator<S>::ParallelOMPEvaluator(const int &nThreads)
    : numberOfThreads(nThreads) {
    omp_set_num_threads(numberOfThreads);
}

/**
 * Método para evaluar una población de individuos en paralelo usando OpenMP
 * @tparam varsT
 * @tparam objsT
 * @param population
 * @return
 */
template <class S>
void ParallelOMPEvaluator<S>::evaluate(vector<S> &population,
                                       Problem<S> *problem) {
    unsigned int index;
    omp_set_num_threads(numberOfThreads);
#pragma omp parallel for private(index)
    for (index = 0; index < population.size(); index++) {
        problem->evaluate(population[index]);
    }
}

#endif  // DIGNEA_PARALLELOMPEVALUATOR_H
