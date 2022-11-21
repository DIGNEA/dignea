/**
 * @file EGenerational.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2021-21-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_EGENERATIONAL_H
#define DIGNEA_EGENERATIONAL_H

#include <dignea/core/Replacement.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/exceptions/NotImplemented.h>

#include <string>

using namespace std;

/**
 * @brief Elitist Generational Replacement Operator
 * Performs the generational replacement operator with elitism 1
 * @tparam S
 */
template <class S>
class EGenerational : public Replacement<S> {
   public:
    EGenerational() = default;

    virtual ~EGenerational() = default;

   public:
    vector<S> replace(vector<S> &, vector<S> &) override;

    vector<S> replace(vector<S> &, const S &, const int &position) override;

    string getName() const override { return "EGenerational"; }
};

/**
 * @brief Performs the generational replacement operator between the two vectors
 * of individuals. The resulting vector is the population with the best fittest
 * solutions and pop.len() - 1 best others.
 * @tparam S
 * @param vector1
 * @return
 */
template <class S>
vector<S> EGenerational<S>::replace(vector<S> &population,
                                    vector<S> &offspring) {
    if (population.empty() || offspring.empty()) {
        std::string what = "vector of individuals empty";
        throw(runtime_error("EGenerational Replacement" + what));
    }
    if (population.size() != offspring.size()) {
        std::string what =
            "population.siz() = " + to_string(population.size()) +
            " != offspring.size = " + to_string(offspring.size());
        throw(runtime_error("EGenerational Replacement" + what));
    }
    // Nos quedamos con el mejor de todos --> elitismo
    sortByFitness(population);
    for (unsigned int i = 1; i < population.size(); i++) {
        population[i] = offspring[i];
    }
    return population;
}
/**
 * @brief Not implemented for this replacement.
 *
 * @tparam S
 * @param population
 * @param offspring
 * @param position
 * @return vector<S>
 */
template <class S>
vector<S> EGenerational<S>::replace(vector<S> &population, const S &offspring,
                                    const int &position) {
    string where =
        "Replace with position not allowed in EGenerational replacement";
    throw NotImplemented(where);
}

#endif  // DIGNEA_EGENERATIONAL_H
