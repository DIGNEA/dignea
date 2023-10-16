/**
 * @file DoubleSphere.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-11-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_DOUBLE_SPHERE_H
#define DIGNEA_DOUBLE_SPHERE_H

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <iostream>
#include <vector>

/**
 * @brief Double Sphere Optimization function
 *
 */
class DoubleSphere : public Problem<FloatSolution> {
   public:
    /// @brief Creates a default DoubleSphere object
    DoubleSphere() = default;

    ~DoubleSphere() override = default;

    /// @brief Creates a DoubleSphere with numberOfVars dimension
    /// @param numberOfVars
    explicit DoubleSphere(const int &numberOfVars);

    /// @brief Creates a DoubleSphere object copying the information from other
    /// instance
    /// @param s
    explicit DoubleSphere(const DoubleSphere *s);

    /// @brief Get the optimization direction for each dimension of the problem
    /// @param i
    /// @return Minimize
    int getOptimizationDirection(int i) const override;

    /// @brief Get the upper limit of the ith dimension.
    /// @param i
    /// @return  5.12
    float getUpperLimit(int i) const override;

    /// @brief Get the lower limit of the ith dimension.
    /// @param i
    /// @return -5.12
    float getLowerLimit(int i) const override;

    /// @brief Get the name of the Problem.
    /// @return a string with the content DoubleSphere
    string getName() const override { return "DoubleSphere"; }

    /// @brief Evaluates a solution with the DoubleSphere equation
    /// @param solution
    void evaluate(FloatSolution &solution) const override;

    /// @brief Not implemented here.
    bool evaluateConstraints(FloatSolution &solution) const override;

    /// @brief Creates a random solution for the DoubleSphere problem
    /// @return
    FloatSolution createSolution() const override;

    /// \deprecated @brief Creates a random solution for the DoubleSphere
    /// problem
    /// @return
    FloatSolution createSolution(ParallelPRNG &engine) const override;

    /// @brief Not implemented here.
    /// @param path
    void readData(const string &path) override;
};

#endif  // DIGNEA_SPHERE_H
