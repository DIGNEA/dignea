/**
 * @file BPP.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __DIGNEA_BPP_H__
#define __DIGNEA_BPP_H__

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <fstream>
#include <iostream>
#include <utility>

/**
 * @brief Class to represent the Bin Packing Problem
 *
 */
class BPP : public Problem<IntIntSolution> {
   public:
    BPP();

    virtual ~BPP() = default;

    BPP(const string &pathToInstance);

    BPP(const int &numberOfVars, const int &capacity, const vector<int> &items);

    BPP(const BPP *);

    float getUpperLimit(int i) const override;

    int getOptimizationDirection(const int i) const override;

    float getLowerLimit(int i) const override;

    string getName() const override { return "Bin-Packing Problem"; };

    void evaluate(IntIntSolution &solution) const override;

    bool evaluateConstraints(IntIntSolution &ptr) const override {
        return true;
    };

    IntIntSolution createSolution() const override;

    IntIntSolution createSolution(ParallelPRNG &engine) const override;

    int getCapacity() const { return capacity; };

    void setCapacity(int capacity);

    const vector<int> &getItems() const { return items; };

    void readData(const string &path) override;

    json to_json() const override;

   protected:
    // Set of items to pack
    vector<int> items;
    // Maximum capacity of each bin
    int capacity;
    std::string pathToInstance;
};

#endif  // __DIGNEA_BPP_H__