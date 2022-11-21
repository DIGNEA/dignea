//
// Created by amarrero on 29/1/21.
//

#include "dignea/problems/KPNR.h"

KPNR::KPNR() : KP() {}

KPNR::~KPNR() {}

KPNR::KPNR(const int &numberOfVars) : KP(numberOfVars) {}

KPNR::KPNR(const string &pathToInstance) : KP(pathToInstance) {}

KPNR::KPNR(const int &numberOfVars, const int &lowerWeight,
           const int &upperWeight, const int &lowerProfit,
           const int &upperProfit)
    : KP(numberOfVars, lowerWeight, upperWeight, lowerProfit, upperProfit) {}

/**
 * Evalua una solucion al problema de la mochila
 * @param solution
 */
void KPNR::evaluate(BoolFloatSolution &solution) const {
    float fitness = 0;
    this->evaluateConstraints(solution);
    vector<char> variables = solution.getVariables();
    for (int i = 0; i < this->numberOfVars; i++) {
        fitness += variables[i] * profits[i];
    }
    // Restamos la posible penalizacion
    fitness -= solution.getConstAt(0);
    solution.setFitness(fitness);
    solution.setObjAt(0, fitness);
}

KPNR::KPNR(const int &numberOfVars, const int &capacity,
           const vector<int> &newWeights, const vector<int> &newProfits)
    : KP(numberOfVars) {
    this->profits = newProfits;
    this->weights = newWeights;
    this->capacity = capacity;
}
