

#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Abstract Distance Class to create an interface which can be use in
 * different algorithms.
 *
 * @tparam T
 */
template <typename T>
class Distance {
   public:
    Distance() = default;
    virtual ~Distance() = default;

    /**
     * @brief Computes the distance between the vectors of type T and returns
     * the value. This method must be implemented in the subclasses.
     *
     * @return T
     */
    virtual T compute(const vector<T>&, const vector<T>&) = 0;
};

#endif