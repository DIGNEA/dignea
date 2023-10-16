/**
 * @file PisPerformance.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <dignea/generator/evaluations/PisPerformance.h>

#include <algorithm>

using namespace std;

/**
 * Computes the fitness of an individual in the EIG.
 * This particular formulation is based on the publicacion of Coello 2019
 * about Knapsack Instance Generation using Heuristics
 * @param solutions
 * @return
 */
float PisPerformance::compute(const vector<float> &solutions) const {
    float targetRunTime = solutions[0];
    float minOthers = *min_element(solutions.begin() + 1, solutions.end());
#ifdef DEBUG
    cout << "Target: " << targetRunTime << endl;
    cout << "MinOthers: " << minOthers << endl;
    cout << "Fitness: " << (minOthers - targetRunTime) << endl;
#endif
    return (minOthers - targetRunTime);
}
