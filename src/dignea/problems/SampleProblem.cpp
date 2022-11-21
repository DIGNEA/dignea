//
// Created by amarrero on 15/12/20.
//

#include "dignea/problems/SampleProblem.h"

#include <dignea/utilities/exceptions/NotImplemented.h>

SampleProblem::SampleProblem(const int &numberOfVars)
    : Problem(numberOfVars, 2, 0) {}

SampleProblem::SampleProblem(const SampleProblem *sample) : Problem(sample) {}

bool SampleProblem::evaluateConstraints(FloatSolution &solution) const {
    std::string where = "evaluateConstraints at SampleProblem";
    throw(NotImplemented(where));
}

void SampleProblem::readData(const string &path) {
    std::string where = "readData at SampleProblem";
    throw(NotImplemented(where));
}

void SampleProblem::evaluate(FloatSolution &solution) const {
    std::string where = "evaluate at SampleProblem";
    throw(NotImplemented(where));
}
