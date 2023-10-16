/**
 * @file ZDT.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-11-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __DIGNEA_ZDT_H__
#define __DIGNEA_ZDT_H__

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <iostream>
#include <vector>

enum class ZDT_MODE { ZDT1, ZDT2, ZDT3, ZDT4, ZDT6 };

/**
 * @brief Class to represent the family of ZDT functions
 *
 */
class ZDT : public Problem<FloatSolution> {
   public:
    /// @brief Creates a default ZDT object
    ZDT() = default;

    ~ZDT() override = default;

    /// @brief Creates a ZDT with numberOfVars dimension
    /// @param numberOfVars
    explicit ZDT(ZDT_MODE mode, const int &numberOfVars);

    /// @brief Creates a ZDT object copying the information from other
    /// instance
    /// @param s
    explicit ZDT(const ZDT *s);

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
    /// @return a string with the content ZDT
    string getName() const override { return "ZDT"; }

    /// @brief Evaluates a solution with the ZDT equation
    /// @param solution
    void evaluate(FloatSolution &solution) const override;

    /// @brief Not implemented here.
    bool evaluateConstraints(FloatSolution &solution) const override;

    /// @brief Creates a random solution for the ZDT problem
    /// @return
    FloatSolution createSolution() const override;

    /// \deprecated @brief Creates a random solution for the ZDT problem
    /// @return
    FloatSolution createSolution(ParallelPRNG &engine) const override;

    /// @brief Not implemented here.
    /// @param path
    void readData(const string &path) override;

   private:
    ZDT_MODE mode;
};

#endif