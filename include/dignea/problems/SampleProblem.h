//
// Created by amarrero on 15/12/20.
//

#ifndef DIGNEA_SAMPLEPROBLEM_H
#define DIGNEA_SAMPLEPROBLEM_H

/**
 * SampleProblem.h
 *
 * Author: Alejandro Marrero
 * Contact: alejandro.marrero.10@ull.edu.es
 *
 *  12 - 2020
 **/

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <iostream>

/**
 * Problema de ejemplo empleado para realizar tests.
 * El objetivo numero 0 es para minimizar y el resto para maximizar
 */
class SampleProblem : public Problem<FloatSolution> {
   public:
    SampleProblem() = default;

    virtual ~SampleProblem() override = default;

    explicit SampleProblem(const int &numberOfVars);

    explicit SampleProblem(const SampleProblem *sample);

    int getOptimizationDirection(const int i) const override {
        if (i == 0) {
            return Minimize;
        } else {
            return Maximize;
        }
    }

    float getUpperLimit(const int i) const override { return -1; }

    float getLowerLimit(const int i) const override { return -1; };

    string getName() const override { return "SampleProblem"; };

    bool evaluateConstraints(FloatSolution &ptr) const override;

    void evaluate(FloatSolution &solution) const override;

    FloatSolution createSolution() const override { return FloatSolution(); }

    FloatSolution createSolution(ParallelPRNG &engine) const override {
        return FloatSolution();
    }

    void readData(const string &path) override;
};

#endif