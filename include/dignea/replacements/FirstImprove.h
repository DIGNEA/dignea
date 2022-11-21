/**
 * @file FirstImprove.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2021-21-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_FIRSTIMPROVE_H
#define DIGNEA_FIRSTIMPROVE_H

#include <dignea/core/Replacement.h>
#include <dignea/utilities/Comparator.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/exceptions/NotImplemented.h>

#include <string>

using namespace std;

/**
 * @brief Replacement procedure which compares the individuals of the current
 * population with the offspring individuals. For each individual i, the
 * offspring and the parent at the ith position are compared and the fittest
 * survives.
 * @tparam S
 */
template <class S>
class FirstImprove : public Replacement<S> {
   public:
    FirstImprove() = default;

    virtual ~FirstImprove() = default;

    vector<S> replace(vector<S> &vector1, vector<S> &vector2) override;

    vector<S> replace(vector<S> &vector1, const S &solution,
                      const int &position) override;

    string getName() const override { return "First Improve"; }
};

/**
 * @brief First Improve replacement logic. Loops both vector of individuals
 * doing pairwise comparisons for each ith individual in both vectors. The final
 * population contains the best individuals at each ith position.
 * @tparam S
 * @param population
 * @param offspring
 * @return
 */
template <class S>
vector<S> FirstImprove<S>::replace(vector<S> &population,
                                   vector<S> &offspring) {
    if (population.empty() || offspring.empty()) {
        std::string what = "Empty vector of individuals";
        throw(runtime_error("Replacement in First Improve" + what));
    }
    if (population.size() != offspring.size()) {
        std::string what =
            "popSize = " + to_string(population.size()) +
            " != offspring.size = " + to_string(offspring.size());
        throw(runtime_error("Replacement in First Improve" + what));
    }
    for (unsigned int i = 0; i < population.size(); i++) {
        // Improves the constraints?
        if (offspring[i].getNumberOfCons() != 0) {
            for (int j = 0; j < offspring[i].getNumberOfCons(); j++) {
                if (offspring[i].getConstAt(j) < population[i].getConstAt(j)) {
                    population[i] = offspring[i];
                    break;  // Improves --> continue with the next individual
                }
            }
        } else {
            if (cmpByFitness(offspring[i], population[i]) == FIRST_BEST) {
                population[i] = offspring[i];
            }
        }
    }
    return population;
}

/**
 * @brief Not implemented for this replacement.
 *
 * @tparam S
 * @param vector1
 * @param solution
 * @param position
 * @return vector<S>
 */
template <class S>
vector<S> FirstImprove<S>::replace(vector<S> &vector1, const S &solution,
                                   const int &position) {
    string where =
        "Replace with position not allowed in First Improve replacement";
    throw NotImplemented(where);
}

#endif  // DIGNEA_FIRSTIMPROVE_H
