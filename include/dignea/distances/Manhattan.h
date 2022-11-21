
#ifndef __MANHATTAN_H__
#define __MANHATTAN_H__

#include <dignea/utilities/exceptions/Mismatch.h>

#include <algorithm>
#include <cmath>
#include <numeric>

#include "Distance.h"

/**
 * @brief Manhattan Distance. The template parameter for this class is set to
 * float.
 * @tparam T
 */
template <typename T = float>
class Manhattan : public Distance<T> {
   public:
    Manhattan() = default;
    virtual ~Manhattan() = default;

    T compute(const vector<T>&, const vector<T>&);
};

/**
 * @brief Computes the Manhattan distance between the two given vectors.
 *
 * @tparam T
 * @param ind1
 * @param ind2
 * @return T
 */
template <typename T>
T Manhattan<T>::compute(const vector<T>& ind1, const vector<T>& ind2) {
    if (ind1.size() != ind2.size()) {
        string where = "individuals sizes in Manhattan::distanceBetween";
        throw(Mismatch(where));
    }
    T distance = {};
    for (unsigned int i = 0; i < ind1.size(); i++) {
        distance += fabs((ind1[i] - ind2[i]));
    }
    return distance;
}

#endif  //__Manhattan_H__