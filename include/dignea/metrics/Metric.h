//
// Created by amarrero on 25/3/21.
//

#ifndef DIGNEA_METRIC_H
#define DIGNEA_METRIC_H

/**
 * Metric.h
 *
 * Author: Alejandro Marrero
 * Contact: alejandro.marrero.10@ull.edu.es
 *
 *  03 - 2021
 **/

#include <dignea/core/Front.h>

#include <memory>

using namespace std;

template <typename T, typename S>
class Metric {
   public:
    Metric() = default;

    virtual ~Metric() = default;

    virtual T computeValue(const Front<S>& front) const = 0;

    virtual T computeValue(const vector<T>& sols) const = 0;
};

#endif
