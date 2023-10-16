/**
 * @file BPPDomain.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef DIGNEA_BPPDOMAIN_H
#define DIGNEA_BPPDOMAIN_H

#include <dignea/generator/AbstractDomain.h>
#include <dignea/generator/instances/BPPInstance.h>
#include <dignea/problems/BPP.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <random>
#include <ranges>
/**
 * @brief Instance Generator for Bin-Packing Problem.
 *
 * The number of variables (dimension) is set as the number of variables
 * which it is used to create an object (N items).The maximum capacity of the
 *bins, is calculated as the 80% of the weights from the bins
 *
 *
 **/
class BPPDomain : public AbstractDomain<BPP, BPPInstance> {
   public:
    BPPDomain();

    explicit BPPDomain(const int &numberOfVars, const int &numberOfObjs = 1,
                       const int &capacity = 100);

    virtual ~BPPDomain() = default;

    BPPDomain(const string &pathToInstance) = delete;

    BPPDomain(const int &numberOfVars, const int &numberOfObjs,
              const int &capacity, const int &minW, const int &maxW);

    shared_ptr<BPP> genOptProblem(const BPPInstance &instance) const override;

    void evaluate(BPPInstance &solution) const override;

    vector<BPPInstance> createSolutions(const int &maxSolutions) const override;

    void beforeEvaluation(vector<BPPInstance> &) override;

    void afterEvaluation(vector<BPPInstance> &) override;

    BPPInstance createSolution() const override;

    BPPInstance createSolution(ParallelPRNG &engine) const override;

    bool evaluateConstraints(BPPInstance &solution) const override {
        return true;
    }

    int getOptimizationDirection(const int i) const override;

    string getName() const override { return "BPPDomain"; }

    float getUpperLimit(const int i) const override;

    float getLowerLimit(const int i) const override;

    int getMaxWeight() const { return maxWeight; }

    int getMinWeight() const { return minWeight; }

    void setMaxWeight(int maxWeight) { BPPDomain::maxWeight = maxWeight; }

    void setMinWeight(int minWeight) { BPPDomain::minWeight = minWeight; }

    json to_json() const override;

   private:
    BPPInstance createSolution(const int &instanceIndex,
                               const int &maxInstances) const;

   protected:
    int maxWeight;
    int minWeight;
    int defaultCapacity;

   public:
    // Unimplemented Methods
    void readData(const string &path) override{};
};

#endif