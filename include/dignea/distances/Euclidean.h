
#ifndef __EUCLIDEAN_H__
#define __EUCLIDEAN_H__

#include <dignea/utilities/exceptions/Mismatch.h>

#include <algorithm>
#include <cmath>

#include "Distance.h"

/**
 * @brief Euclidean distance.
 *
 * @tparam T
 */
template <typename T>
class Euclidean : public Distance<T> {
   public:
    Euclidean() = default;

    virtual ~Euclidean() = default;

    T compute(const vector<T>&, const vector<T>&);

   private:
    template <typename I1, typename I2>
    T euclidean(I1 first1, I1 last1, I2 first2);
};

/**
 * @brief Computes the Euclidean distance between the two vectors.
 *
 * @tparam T
 * @param ind1
 * @param ind2
 * @return T
 */
template <typename T>
T Euclidean<T>::compute(const vector<T>& ind1, const vector<T>& ind2) {
    if (ind1.size() != ind2.size()) {
        string where = "individuals sizes in Euclidean::compute";
        throw(Mismatch(where));
    }
    return euclidean(begin(ind1), end(ind1), begin(ind2));
}

template <typename T>
template <typename I1, typename I2>
T Euclidean<T>::euclidean(I1 first, I1 last, I2 first2) {
    T distance = {};
    while (first != last) {
        double dist = (*first++) - (*first2++);
        distance += dist * dist;
    }
    return distance > 0.0 ? sqrt(distance) : 0.0;
}

#endif  //__EUCLIDEAN_H__