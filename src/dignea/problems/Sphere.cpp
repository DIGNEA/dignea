//
// Created by amarrero on 15/12/20.
//

#include "dignea/problems/Sphere.h"

#include <dignea/utilities/exceptions/NotImplemented.h>

Sphere::Sphere(const int &numberOfVars) : Problem(numberOfVars, 1, 0) {}

Sphere::Sphere(const Sphere *s)
    : Problem<FloatSolution>(s->numberOfVars, 1, 0) {}

void Sphere::evaluate(FloatSolution &solution) const {
    float evaluation = 0.0;
    std::vector<float> x = {solution.getVariables()};
    for (int i = 0; i < solution.getNumberOfVars(); i++) {
        evaluation += (x[i] * x[i]);
    }
    solution.setObjAt(0, evaluation);
}

/**
 * Crea una solucion factible al problema Sphere
 * @return
 */
FloatSolution Sphere::createSolution() const {
    FloatSolution solution(this->numberOfVars, 1, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(PseudoRandom::randDouble(-5.12, 5.12));
    }
    solution.setVariables(vars);
    return solution;
}

float Sphere::getLowerLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where = "getLowerLimit at Sphere with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return -5.12;
}

float Sphere::getUpperLimit(const int i) const {
    if (i < 0 || i >= this->getNumberOfVars()) {
        string where = "getUpperLimit at Sphere with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 5.12;
}

int Sphere::getOptimizationDirection(const int i) const {
    if (i != 0) {
        string where = "getOptimization at Sphere with dim = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

void Sphere::readData(const string &path) {
    string where = "readData in Sphere";
    throw(NotImplemented(where));
}

bool Sphere::evaluateConstraints(FloatSolution &solution) const {
    std::string where = "evaluateConstraints at Sphere";
    throw(NotImplemented(where));
}

FloatSolution Sphere::createSolution(ParallelPRNG &engine) const {
    FloatSolution solution(this->numberOfVars, 1, 0);
    vector<float> vars;
    vars.reserve(this->numberOfVars);

    for (int i = 0; i < this->numberOfVars; i++) {
        vars.push_back(engine.randDouble(-5.12, 5.12));
    }
    solution.setVariables(vars);
    return solution;
}
