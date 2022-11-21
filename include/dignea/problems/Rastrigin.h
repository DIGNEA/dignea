/**
 * @file Rastrigin.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2021-06-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_RASTRIGIN_H
#define DIGNEA_RASTRIGIN_H

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <vector>

/**
 * @brief Rastrigin Optimization function
 *
 */
class Rastrigin : public Problem<FloatSolution> {
   public:
    Rastrigin() = default;

    ~Rastrigin() override = default;
    /**
     * @brief Construct a new Rastrigin with a dimension of numberOfVars
     *
     * @param numberOfVars
     */
    explicit Rastrigin(const int &numberOfVars);
    /**
     * @brief Construct a new Rastrigin object copying the information from the
     * *ras pointer.
     *
     * @param ras
     */
    explicit Rastrigin(const Rastrigin *ras);
    /**
     * @brief Get the Optimization Direction. Only available for one dimension
     * (0) and it returns Minimize.
     *
     * @param i
     * @return int
     */
    int getOptimizationDirection(int i) const override;
    /**
     * @brief Get the upper limit of the Rastrigin function (5.12).
     *
     * @param i
     * @return float
     */
    float getUpperLimit(int i) const override;
    /**
     * @brief Get the lower limit of the Rastrigin function (-5.12).
     *
     * @param i
     * @return float
     */
    float getLowerLimit(int i) const override;
    /**
     * @brief Get the Name
     *
     * @return string
     */
    string getName() const override { return "Rastrigin"; }
    /**
     * @brief Evaluates an individual with the Rastrigin Function
     *
     * @param solution
     */
    void evaluate(FloatSolution &solution) const override;
    /**
     * @brief Not implemented here.
     *
     * @param solution
     * @return true
     * @return false
     */
    bool evaluateConstraints(FloatSolution &solution) const override;
    /**
     * @brief Creates a random solution with values in the range [-5.12, 5.12]
     *
     * @return FloatSolution
     */
    FloatSolution createSolution() const override;
    /**
     * @brief Creates a random solution with values in the range [-5.12, 5.12]
     *
     * @param engine
     * @return FloatSolution
     */
    FloatSolution createSolution(ParallelPRNG &engine) const override;
    /**
     * @brief Not implemented here.
     *
     * @param path
     */
    void readData(const string &path) override;
};

#endif  // DIGNEA_RASTRIGIN_H
