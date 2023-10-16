/**
 * @file DoubleSphere.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-11-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "dignea/problems/DoubleSphere.h"

#include <dignea/utilities/exceptions/NotImplemented.h>

DoubleSphere::DoubleSphere(const int &numberOfVars)
    : Problem(numberOfVars, 2, 0) {}

DoubleSphere::DoubleSphere(const DoubleSphere *s)
    : Problem<FloatSolution>(s->numberOfVars, 2, 0) {}

void DoubleSphere::evaluate(FloatSolution &solution) const {
    float evaluation = 0.0;
    std::vector<float> x = {solution.getVariables()};
    for (int i = 0; i < solution.getNumberOfVars(); i++) {
        evaluation += (x[i] * x[i]);
    }
    solution.setObjAt(0, evaluation);
    solution.setObjAt(1, evaluation);
}

/**
 * Crea una solucion factible al problema DoubleSphere
 * @return
 */
FloatSolution DoubleSphere::createSolution() const {
    FloatSolution solution(this->numberOfVars, 2, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(PseudoRandom::randDouble(-5.12, 5.12));
    }
    solution.setVariables(vars);
    return solution;
}

float DoubleSphere::getLowerLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where =
            "getLowerLimit at DoubleSphere with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return -5.12;
}

float DoubleSphere::getUpperLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where =
            "getUpperLimit at DoubleSphere with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 5.12;
}

int DoubleSphere::getOptimizationDirection(const int i) const {
    if (i > 1) {
        string where =
            "getOptimization at DoubleSphere with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

void DoubleSphere::readData(const string &path) {
    string where = "readData in DoubleSphere";
    throw(NotImplemented(where));
}

bool DoubleSphere::evaluateConstraints(FloatSolution &solution) const {
    std::string where = "evaluateConstraints at DoubleSphere";
    throw(NotImplemented(where));
}

FloatSolution DoubleSphere::createSolution(ParallelPRNG &engine) const {
    FloatSolution solution(this->numberOfVars, 2, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(engine.randDouble(-5.12, 5.12));
    }
    solution.setVariables(vars);
    return solution;
}
