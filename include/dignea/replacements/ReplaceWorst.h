/**
 * @file ReplaceWorst.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2021-21-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_REPLACEWORST_H
#define DIGNEA_REPLACEWORST_H

#include <dignea/core/Replacement.h>
#include <dignea/utilities/Comparator.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/exceptions/NotImplemented.h>

#include <string>

using namespace std;

/**
 * @brief Replacement procedure which replaces the worst individual in the
 * population with the new offspring.
 *
 * @tparam S
 */
template <class S>
class ReplaceWorst : public Replacement<S> {
   public:
    ReplaceWorst() = default;

    virtual ~ReplaceWorst() = default;

    vector<S> replace(vector<S> &vector1, vector<S> &vector2) override;

    vector<S> replace(vector<S> &vector1, const S &solution,
                      const int &position) override;

    string getName() const override { return "Replace Worst"; }
};

/**
 * @brief Replaces the worst individual in the population for the best in
 * offspring. Offspring must be of size 1.
 * @tparam S
 * @param vector1
 * @param vector2
 * @return
 */
template <class S>
vector<S> ReplaceWorst<S>::replace(vector<S> &population,
                                   vector<S> &offspring) {
    if (offspring.empty() || offspring.size() != 1) {
        std::string what = " offspring.size = " + to_string(offspring.size());
        throw(runtime_error("Replace Worst " + what));
    }
    if (population.empty()) {
        std::string what = " population is empty";
        throw(runtime_error("Replace Worst " + what));
    }
    // Buscamos los dos mejores fitness
    sortByFitness(population);
    int worstSIndex = population.size() - 1;
    if (cmpByFitness(population[worstSIndex], offspring[0])) {
        population[worstSIndex] = offspring[0];
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
vector<S> ReplaceWorst<S>::replace(vector<S> &vector1, const S &solution,
                                   const int &position) {
    string where =
        "Replace with position not allowed in Replace Worst replacement";
    throw NotImplemented(where);
}

#endif  // DIGNEA_REPLACEWORST_H
