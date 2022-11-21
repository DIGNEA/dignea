/**
 * @file Sphere.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2019-10-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_SPHERE_H
#define DIGNEA_SPHERE_H

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <iostream>
#include <vector>

/**
 * @brief Sphere Optimization function
 *
 */
class Sphere : public Problem<FloatSolution> {
   public:
    /// @brief Creates a default Sphere object
    Sphere() = default;

    ~Sphere() override = default;

    /// @brief Creates a Sphere with numberOfVars dimension
    /// @param numberOfVars
    explicit Sphere(const int &numberOfVars);

    /// @brief Creates a Sphere object copying the information from other
    /// instance
    /// @param s
    explicit Sphere(const Sphere *s);

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
    /// @return a string with the content Sphere
    string getName() const override { return "Sphere"; }

    /// @brief Evaluates a solution with the Sphere equation
    /// @param solution
    void evaluate(FloatSolution &solution) const override;

    /// @brief Not implemented here.
    bool evaluateConstraints(FloatSolution &solution) const override;

    /// @brief Creates a random solution for the Sphere problem
    /// @return
    FloatSolution createSolution() const override;

    /// \deprecated @brief Creates a random solution for the Sphere problem
    /// @return
    FloatSolution createSolution(ParallelPRNG &engine) const override;

    /// @brief Not implemented here.
    /// @param path
    void readData(const string &path) override;
};

#endif  // DIGNEA_SPHERE_H
