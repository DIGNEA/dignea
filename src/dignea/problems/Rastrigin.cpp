//
// Created by amarrero on 25/6/21.
//

#include "dignea/problems/Rastrigin.h"

#include <dignea/utilities/exceptions/NotImplemented.h>

#include <cmath>

Rastrigin::Rastrigin(const int &numberOfVars)
    : Problem<FloatSolution>(numberOfVars, 1, 0) {}

Rastrigin::Rastrigin(const Rastrigin *ras)
    : Problem<FloatSolution>(ras->numberOfVars, 1, 0) {}

void Rastrigin::evaluate(FloatSolution &solution) const {
    double evaluation = 0.0;
    std::vector<float> vars = {solution.getVariables()};
    for (const float &x : vars) {
        evaluation += ((x * x) - 10.0 * cos(2.0 * M_PI * x));
    }
    evaluation = 10.0 * solution.getNumberOfVars() + evaluation;
    solution.setObjAt(0, evaluation);
    solution.setFitness(evaluation);
}

/**
 * Crea una solucion factible al problema Rastrigin
 * @return
 */
FloatSolution Rastrigin::createSolution() const {
    FloatSolution solution(this->numberOfVars, 1, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(PseudoRandom::randDouble(-5.12, 5.12));
    }
    solution.setVariables(vars);
    return solution;
}

float Rastrigin::getLowerLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where = "getLowerLimit at Rastrigin with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return -5.12;
}

float Rastrigin::getUpperLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where = "getUpperLimit at Rastrigin with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 5.12;
}

int Rastrigin::getOptimizationDirection(const int i) const {
    if (i != 0) {
        string where =
            "getOptimization at Rastrigin with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

void Rastrigin::readData(const string &path) {
    string where = "readData in Rastrigin";
    throw(NotImplemented(where));
}

bool Rastrigin::evaluateConstraints(FloatSolution &solution) const {
    std::string where = "evaluateConstraints at Rastrigin";
    throw(NotImplemented(where));
}

FloatSolution Rastrigin::createSolution(ParallelPRNG &engine) const {
    FloatSolution solution(this->numberOfVars, 1, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(engine.randDouble(-5.12, 5.12));
    }
    solution.setVariables(vars);
    return solution;
}
