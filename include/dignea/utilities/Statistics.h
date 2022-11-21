/**
 * @file Statistics.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2021-06-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_STATISTICS_H
#define DIGNEA_STATISTICS_H

#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>
#include <vector>

/**
 * @brief Computes the mean value of the given vector. Returns NaN if empty
 * @tparam T
 * @param variables
 * @return
 */
template <typename T>
double mean(const vector<T> &variables) {
    if (variables.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return std::accumulate(variables.begin(), variables.end(), 0.0) /
           variables.size();
}

/**
 * @brief Computes the mean value of the given matrix. Returns NaN if empty
 * @tparam T
 * @param variables
 * @return
 */
template <typename T>
double mean(const vector<vector<T>> &matrix) {
    if (matrix.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double size = matrix.size() * matrix[0].size();
    auto sum = std::accumulate(
        matrix.cbegin(), matrix.cend(), 0, [](auto lhs, const auto &rhs) {
            return std::accumulate(rhs.cbegin(), rhs.cend(), lhs);
        });
    return sum / size;
}

/**
 * @brief Computes the variance of the vector based on the variables and the
 * mean. Returns NaN if empty.
 * @tparam T
 * @param mean
 * @param variables
 * @return
 */
template <typename T>
double variance(const double &mean, const vector<T> &variables) {
    if (variables.size() <= 1u) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    auto const square = [mean](double sum, int i) {
        auto d = i - mean;
        return sum + d * d;
    };
    double total =
        std::accumulate(variables.begin(), variables.end(), 0.0, square);
    return total / (variables.size());
}

/**
 * @brief Computes the variance of the matrix based on the variables and the
 * mean. Returns NaN if empty.
 * @tparam T
 * @param mean
 * @param variables
 * @return
 */
template <typename T>
double variance(const double &mean, const vector<vector<T>> &matrix) {
    if (matrix.size() <= 1u) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double size = matrix.size() * matrix[0].size();

    double result = 0.0;
    for (const auto &row : matrix) {
        for (const T &value : row) {
            auto d = value - mean;
            result += d * d;
        }
    }
    return result / size;
}

/**
 * @brief Computes the Standard Deviation of the vector based on the mean value.
 * Returns NaN if empty.
 * @tparam T
 * @param mean
 * @param variables
 * @return
 */
template <typename T>
double standardDev(double mean, const vector<T> &variables) {
    return std::sqrt(variance(mean, variables));
}

/**
 * @brief Computes the Standard Deviation of the matrix based on the mean value.
 * Returns NaN if empty.
 * @tparam T
 * @param mean
 * @param variables
 * @return
 */
template <typename T>
double standardDev(double mean, const vector<vector<T>> &matrix) {
    return std::sqrt(variance(mean, matrix));
}

#endif  // DIGNEA_STATISTICS_H
