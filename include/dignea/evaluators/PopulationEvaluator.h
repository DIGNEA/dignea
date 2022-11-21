//
// Created by amarrero on 2/12/20.
//

#ifndef DIGNEA_POPULATIONEVALUATOR_H
#define DIGNEA_POPULATIONEVALUATOR_H

#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>

#include <vector>

using namespace std;

/**
 * Clase que evalua un poblacion de soluciones a un problema concreto
 * @tparam Problem
 * @tparam S
 */
template <class S>
class PopulationEvaluator {
   public:
    PopulationEvaluator() = default;

    virtual ~PopulationEvaluator() = default;

    virtual void evaluate(vector<S> &population, Problem<S> *);

    virtual void evaluate(S &solution, Problem<S> *problem);

    virtual std::string getName() const {
        return "Sequential Population Evaluator";
    }
};

template <class S>
void PopulationEvaluator<S>::evaluate(vector<S> &population,
                                      Problem<S> *problem) {
    for (S &ind : population) {
        problem->evaluate(ind);
    }
}

template <class S>
void PopulationEvaluator<S>::evaluate(S &solution, Problem<S> *problem) {
    problem->evaluate(solution);
}

#endif  // DIGNEA_POPULATIONEVALUATOR_H
