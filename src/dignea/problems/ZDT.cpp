/**
 * @file ZDT.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-11-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "dignea/problems/ZDT.h"

#include <dignea/utilities/exceptions/NotImplemented.h>
#include <math.h>

ZDT::ZDT(ZDT_MODE mode, const int &numberOfVars)
    : Problem(numberOfVars, 2, 0), mode(mode) {}

ZDT::ZDT(const ZDT *s)
    : Problem<FloatSolution>(s->numberOfVars, 2, 0), mode(s->mode) {}

void ZDT::evaluate(FloatSolution &solution) const {
    float evaluation = 0.0;
    if (this->mode == ZDT_MODE::ZDT1) {
        vector<float> x = {solution.getVariables()};
        solution.setObjAt(0, x[0]);
        for (int i = 1; i < this->getNumberOfVars(); i++) {
            evaluation += x[i];
        }
        auto g = 1.0 + (9.0 / (this->getNumberOfVars() - 1)) * evaluation;
        auto f2 = g * (1 - sqrt(x[0] / g));
        solution.setObjAt(1, f2);
    }
}

/**
 * Crea una solucion factible al problema ZDT
 * @return
 */
FloatSolution ZDT::createSolution() const {
    FloatSolution solution(this->numberOfVars, 2, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(PseudoRandom::randDouble(0.0, 1.0));
    }
    solution.setVariables(vars);
    return solution;
}

float ZDT::getLowerLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where = "getLowerLimit at ZDT with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 1.0;
}

float ZDT::getUpperLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where = "getUpperLimit at ZDT with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 0.0;
}

int ZDT::getOptimizationDirection(const int i) const {
    if (i > 1) {
        string where = "getOptimization at ZDT with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

void ZDT::readData(const string &path) {
    string where = "readData in ZDT";
    throw(NotImplemented(where));
}

bool ZDT::evaluateConstraints(FloatSolution &solution) const {
    std::string where = "evaluateConstraints at ZDT";
    throw(NotImplemented(where));
}

FloatSolution ZDT::createSolution(ParallelPRNG &engine) const {
    FloatSolution solution(this->numberOfVars, 2, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(engine.randDouble(0.0, 1.0));
    }
    solution.setVariables(vars);
    return solution;
}
