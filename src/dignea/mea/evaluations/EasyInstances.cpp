//
// Created by amarrero on 15/4/21.
//
#include <dignea/mea/evaluations/EasyInstances.h>

#include <algorithm>

using namespace std;

/**
 * Computes the fitness of an individual in the MEA.
 * This particular formulation is based on the publicacion of Coello 2019
 * about Knapsack Instance Generation using Heuristics
 * @param solutions
 * @return
 */
float EasyInstances::compute(const vector<float> &solutions) const {
    float targetInstance = solutions[0];
    float maxOthers = *max_element(solutions.begin() + 1, solutions.end());
#ifdef DEBUG
    float fitness{targetInstance - maxOthers};
    std::cout << "Target: " << targetInstance << " max(Others): " << maxOthers
              << " Fitness: " << fitness << std::endl;
#endif
    return (targetInstance - maxOthers);
}
