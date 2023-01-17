/**
 * @file KPDomain.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 8/3/21.
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_EKPPROBLEM_H
#define DIGNEA_EKPPROBLEM_H

#include <dignea/generator/AbstractDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/problems/KPNR.h>
#include <dignea/types/SolutionTypes.h>

/**
 * @brief Instance Generator Knapsack Problem.
 * Problem for the Instance Generator to generate Knapsack Problem Instances
 * The number of variables (dimension) is set as the double of the
 * number of variables which it is used to create an object.
 * This is because the KPInstance must be twice size of the nVars.
 *
 */
class KPDomain : public AbstractDomain<KPNR, KPInstance> {
   public:
    KPDomain();

    explicit KPDomain(const int &numberOfVars);

    virtual ~KPDomain();

    KPDomain(const string &pathToInstance) = delete;

    KPDomain(const int &numberOfVars, const int &numOfInstances,
               const int &lowerWeight, const int &upperWeight,
               const int &lowerProfit, const int &upperProfit);

    shared_ptr<KPNR> genOptProblem(const KPInstance &instance) const override;

    void evaluate(KPInstance &solution) const override;

    vector<KPInstance> createSolutions(const int &maxSolutions) const override;

    void beforeEvaluation(vector<KPInstance> &) override;

    void afterEvaluation(vector<KPInstance> &) override;

    KPInstance createSolution() const override;

    KPInstance createSolution(ParallelPRNG &engine) const override;

    KPInstance createSolution();

    KPInstance createSolution(ParallelPRNG &engine);

    bool evaluateConstraints(KPInstance &solution) const override {
        return true;
    }

    int getOptimizationDirection(const int i) const override;

    string getName() const override { return "KPDomain"; }

    float getUpperLimit(const int i) const override;

    float getLowerLimit(const int i) const override;

    int getNumberOfInstances() const { return numberOfInstances; }

    void setNumberOfInstances(int numberOfInstances) {
        KPDomain::numberOfInstances = numberOfInstances;
    }

    int getLowWeight() const { return lowWeight; }

    void setLowWeight(int lowWeight) { KPDomain::lowWeight = lowWeight; }

    int getUpWeight() const { return upWeight; }

    void setUpWeight(int upWeight) { KPDomain::upWeight = upWeight; }

    int getLowProfit() const { return lowProfit; }

    void setLowProfit(int lowProfit) { KPDomain::lowProfit = lowProfit; }

    int getUpProfit() const { return upProfit; }

    void setUpProfit(int upProfit) { KPDomain::upProfit = upProfit; }

    int getGeneratedInstances() const { return generatedInstances; }

    void setGeneratedInstances(int generatedInstances) {
        KPDomain::generatedInstances = generatedInstances;
    }

   private:
    KPInstance createSolution(const int &instanceIndex,
                               const int &maxInstances) const;

   protected:
    int numberOfInstances;
    int lowWeight;
    int upWeight;
    int lowProfit;
    int upProfit;
    // This allows to call createSolution without args
    int generatedInstances;

   public:
    // Unimplemented Methods
    void readData(const string &path) override{};
};

#endif