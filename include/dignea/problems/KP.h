/**
 * @file KP.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 16/12/20
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __DIGNEA_KNP_H__
#define __DIGNEA_KNP_H__

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <fstream>
#include <iostream>
#include <utility>

using iEff = pair<int, float>;

/**
 * @brief Class representing a Knapsack Problem
 *
 */
class KP : public Problem<BoolFloatSolution> {
   public:
    KP();

    virtual ~KP();

    explicit KP(const int &numberOfVars);

    KP(const string &pathToInstance);

    KP(const int &numberOfVars, const int &capacity, const vector<int> &weights,
       const vector<int> &profits);

    // Generacion de una instancia del KP de manera aleatoria con los datos
    // proporcionados
    KP(const int &numberOfVars, const int &lowerWeight, const int &upperWeight,
       const int &lowerProfit, const int &upperProfit);

    KP(const KP *);

    float getUpperLimit(int i) const override;

    int getOptimizationDirection(const int i) const override;

    float getLowerLimit(int i) const override;

    string getName() const override { return "Knapsack Problem"; };

    void evaluate(BoolFloatSolution &solution) const override;

    bool evaluateConstraints(BoolFloatSolution &ptr) const override;

    BoolFloatSolution createSolution() const override;

    BoolFloatSolution createSolution(ParallelPRNG &engine) const override;

    int getLowWeight() const;

    void setLowWeight(int lowWeight);

    int getUpWeight() const;

    void setUpWeight(int upWeight);

    int getLowProfit() const;

    void setLowProfit(int lowProfit);

    int getUpProfit() const;

    void setUpProfit(int upProfit);

    const vector<int> &getProfits() const;

    void setProfits(const vector<int> &profits);

    const vector<int> &getWeights() const;

    void setWeights(const vector<int> &weights);

    int getCapacity() const;

    void setCapacity(int capacity);

    void readData(const string &path) override;

    json to_json() const override;

    vector<iEff> getItemsSortedByEff() const { return itemsEff; }

   protected:
    void computeEfficiency();

    void repair(BoolFloatSolution &solution) const;

   protected:
    int lowWeight;
    int upWeight;
    int lowProfit;
    int upProfit;
    // Profits, weights and final capacity of the instance
    vector<int> profits;
    vector<int> weights;
    int capacity;
    std::string pathToInstance;
    // Vector con la eficiencia de los elementos de la instancia
    vector<iEff> itemsEff;

   public:
    static const string INSTANCE;
    static const string PROFITS;
    static const string WEIGHTS;
    static const string CAPACITY;
};

#endif  // __DIGNEA_KNP_H__