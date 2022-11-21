
#ifndef __HAMMING_H__
#define __HAMMING_H__

#include <dignea/utilities/exceptions/Mismatch.h>

#include <algorithm>
#include <vector>

#include "Distance.h"

using namespace std;

/**
 * @brief Hamming distance. The template parameter for this class is set to
 * int.
 * @tparam T
 */
template <typename T = int>
class Hamming : public Distance<T> {
   public:
    Hamming() = default;
    virtual ~Hamming() = default;

    T compute(const vector<T>&, const vector<T>&);
};

/**
 * @brief Computes the Hamming Distance between the two given vectors
 *
 * @tparam T
 * @tparam I
 * @param size
 * @param b1
 * @param b2
 * @return T
 */
template <typename T>
T Hamming<T>::compute(const vector<T>& ind1, const vector<T>& ind2) {
    if (ind1.size() != ind2.size()) {
        string where = "individuals sizes in Hamming::distanceBetween";
        throw(Mismatch(where));
    }
    T distance = {};
    for (unsigned int i = 0; i < ind1.size(); i++) {
        if (ind1[i] != ind2[i]) {
            distance++;
        }
    }
    return distance;
}

#endif  // __HAMMING_H__