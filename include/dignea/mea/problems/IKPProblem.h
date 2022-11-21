/**
 * @file IKPProblem.h
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

#include <dignea/mea/MEAProblem.h>
#include <dignea/mea/solutions/IKPSolution.h>
#include <dignea/problems/KPNR.h>
#include <dignea/types/SolutionTypes.h>

/**
 * @brief Instance Generator Knapsack Problem.
 * Problem for the Instance Generator to generate Knapsack Problem Instances
 * The number of variables (dimension) is set as the double of the
 * number of variables which it is used to create an object.
 * This is because the IKPSolution must be twice size of the nVars.
 *
 */
class IKPProblem : public MEAProblem<KPNR, IKPSolution> {
   public:
    IKPProblem();

    explicit IKPProblem(const int &numberOfVars);

    virtual ~IKPProblem();

    IKPProblem(const string &pathToInstance) = delete;

    IKPProblem(const int &numberOfVars, const int &numOfInstances,
               const int &lowerWeight, const int &upperWeight,
               const int &lowerProfit, const int &upperProfit);

    shared_ptr<KPNR> genOptProblem(const IKPSolution &instance) const override;

    void evaluate(IKPSolution &solution) const override;

    vector<IKPSolution> createSolutions(const int &maxSolutions) const override;

    void beforeEvaluation(vector<IKPSolution> &) override;

    void afterEvaluation(vector<IKPSolution> &) override;

    IKPSolution createSolution() const override;

    IKPSolution createSolution(ParallelPRNG &engine) const override;

    IKPSolution createSolution();

    IKPSolution createSolution(ParallelPRNG &engine);

    bool evaluateConstraints(IKPSolution &solution) const override {
        return true;
    }

    int getOptimizationDirection(const int i) const override;

    string getName() const override { return "IKPProblem"; }

    float getUpperLimit(const int i) const override;

    float getLowerLimit(const int i) const override;

    int getNumberOfInstances() const { return numberOfInstances; }

    void setNumberOfInstances(int numberOfInstances) {
        IKPProblem::numberOfInstances = numberOfInstances;
    }

    int getLowWeight() const { return lowWeight; }

    void setLowWeight(int lowWeight) { IKPProblem::lowWeight = lowWeight; }

    int getUpWeight() const { return upWeight; }

    void setUpWeight(int upWeight) { IKPProblem::upWeight = upWeight; }

    int getLowProfit() const { return lowProfit; }

    void setLowProfit(int lowProfit) { IKPProblem::lowProfit = lowProfit; }

    int getUpProfit() const { return upProfit; }

    void setUpProfit(int upProfit) { IKPProblem::upProfit = upProfit; }

    int getGeneratedInstances() const { return generatedInstances; }

    void setGeneratedInstances(int generatedInstances) {
        IKPProblem::generatedInstances = generatedInstances;
    }

   private:
    IKPSolution createSolution(const int &instanceIndex,
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