

#ifndef __DIVERSITY_H__
#define __DIVERSITY_H__

#include <dignea/core/Solution.h>
#include <dignea/distances/Distance.h>
#include <dignea/utilities/exceptions/NotImplemented.h>

#include <vector>

using namespace std;

/**
 * @brief Base class to define the Diversity Measures
 *
 * @tparam T Type of variable to define the measure (float, int, etc)
 */
template <typename T>
class Diversity {
   public:
    Diversity(unique_ptr<Distance<T>> metric);

    virtual ~Diversity() = default;

    template <typename S>
    T compute(vector<S> &);

   private:
    unique_ptr<Distance<T>> metric;
};

/**
 * @brief Construct a new Diversity< T>:: Diversity object
 *
 * @tparam T
 * @param metric
 */
template <typename T>
Diversity<T>::Diversity(unique_ptr<Distance<T>> metric) {
    this->metric = move(metric);
}

/**
 * @brief Computes the average diversity between the individuals in the
 * population
 *
 * @tparam T type of the return
 * @tparam S
 * @param population
 * @return T the average Hamming Distance between the pairs of individuals in
 * the population
 */
template <typename T>
template <typename S>
T Diversity<T>::compute(vector<S> &population) {
    T distance = {};
    for (unsigned int i = 0; i < population.size(); i++) {
        for (unsigned int j = i + 1; j < population.size(); j++) {
            vector ind1 = population[i].getVariables();
            vector ind2 = population[j].getVariables();
            distance += metric->compute(ind1, ind2);
        }
    }
    return distance / population.size();
}

#endif