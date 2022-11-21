//
// Created by amarrero on 8/3/21.
//

#include <dignea/mea/problems/IKPProblem.h>
#include <dignea/mea/solutions/IKPSolution.h>
#include <dignea/utilities/Statistics.h>
#include <dignea/utilities/random/PseudoRandom.h>

IKPProblem::IKPProblem()
    : MEAProblem<KPNR, IKPSolution>(1, 1, 0), generatedInstances(0) {}

IKPProblem::IKPProblem(const int &numberOfVars)
    : MEAProblem<KPNR, IKPSolution>(numberOfVars * 2, 1, 0),
      generatedInstances(0) {}

IKPProblem::~IKPProblem() {}

IKPProblem::IKPProblem(const int &numberOfVars, const int &numOfInstances,
                       const int &lowerWeight, const int &upperWeight,
                       const int &lowerProfit, const int &upperProfit)
    : MEAProblem<KPNR, IKPSolution>(numberOfVars, 1, 0),
      numberOfInstances(numOfInstances),
      lowWeight(lowerWeight),
      upWeight(upperWeight),
      lowProfit(lowerProfit),
      upProfit(upperProfit),
      generatedInstances(0) {}

/**
 * Method which evaluates a solution of the Knapsack Instance Generation Problem
 * @param solution
 */
void IKPProblem::evaluate(IKPSolution &solution) const { return; }

/**
 * Creates a new solution for the Knapsack Instance Generation Problem
 * @param instanceIndex
 * @return
 */
IKPSolution IKPProblem::createSolution(const int &instanceIndex,
                                       const int &maxInstances) const {
    vector<int> vars(this->numberOfVars, 0);
    float sumOfWeights = 0;
    for (int i = 0; i < this->numberOfVars; i += 2) {
        vars[i] = PseudoRandom::randInt(this->lowWeight, this->upWeight);
        vars[i + 1] = PseudoRandom::randInt(this->lowProfit, this->upProfit);
        sumOfWeights += vars[i];
    }
    // Computing the capacity of the KP Instance Solution
    int capacity = (int)0.8 * sumOfWeights;
    //(int)(((float)instanceIndex / (float)(maxInstances + 1)) * sumOfWeights);
    IKPSolution solution(this->numberOfVars);
    solution.setVariables(vars);
    solution.setCapacity(capacity);
    return solution;
}

/**
 * Generates a new solution without args
 * @return
 */
IKPSolution IKPProblem::createSolution() const {
    return this->createSolution(1, numberOfInstances);
}

IKPSolution IKPProblem::createSolution(ParallelPRNG &engine) const {
    return this->createSolution(1, numberOfInstances);
}

/**
 * Generates a new solution without args
 * @return
 */
IKPSolution IKPProblem::createSolution() {
    generatedInstances++;
    return this->createSolution(generatedInstances, numberOfInstances);
}

IKPSolution IKPProblem::createSolution(ParallelPRNG &engine) {
    generatedInstances++;
    return this->createSolution(generatedInstances, numberOfInstances);
}

/**
 * Returns the upper bound for the variable i
 * @param i
 * @return
 */
float IKPProblem::getUpperLimit(const int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getUpperLimit in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    if (i % 2 == 0) {
        return upWeight;
    }
    return upProfit;
}

/**
 * Returns the lower bound for the variable i
 * @param i
 * @return
 */
float IKPProblem::getLowerLimit(const int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getLowerLimit in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    if (i % 2 == 0) {
        return lowWeight;
    }
    return lowProfit;
}

/**
 * Creates a Optimisation Problem for the Configurations on the MEA
 * In this case, KPNR Problem
 * @return
 */
shared_ptr<KPNR> IKPProblem::genOptProblem(const IKPSolution &instance) const {
    vector<int> variables = instance.getVariables();
    if (variables.size() != (unsigned int)this->numberOfVars) {
        string where = "instance.variables.size() != Problem inner dimension";
        throw OutOfRange(where);
    }
    // Splits the variables in two different vectors
    vector<int> problemW;
    vector<int> problemP;
    problemW.reserve(this->numberOfVars / 2);
    problemP.reserve(this->numberOfVars / 2);
    /**
     * Unfolded loop which creates a KPNR problem from a IKPSolution
     * The IKPSolution stores the weights and profits in a single array
     * This loop creates two arrays from the one in the IKPSolution object
     * - variables.size() => instance.getNumberOfVars() * 2
     */
    for (int i = 0; i < this->numberOfVars; i += 2) {
        problemW.push_back(variables[i]);
        problemP.push_back(variables[i + 1]);
    }
    return make_shared<KPNR>(this->numberOfVars / 2, instance.getCapacity(),
                             problemW, problemP);
}

/**
 * Method which returns the optimisation direction for each objective
 * Minimize
 * @param i
 * @return
 */
int IKPProblem::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= this->getNumberOfObjs()) {
        std::string where =
            "getOptimizationDirection in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Maximize;
}

/**
 * Creates a population of solutions
 * @param maxSolutions
 * @return
 */
vector<IKPSolution> IKPProblem::createSolutions(const int &maxSolutions) const {
    vector<IKPSolution> solutions;
    solutions.reserve(maxSolutions);
    for (int i = 1; i <= maxSolutions; i++) {
        solutions.push_back(this->createSolution(i, maxSolutions));
    }
    return solutions;
}

/**
 * Method which re-calculates the capacity of the instances before evaluate them
 * @param vector1
 */
void IKPProblem::beforeEvaluation(vector<IKPSolution> &solutions) {
    int maxInstances = solutions.size();
    int instanceIndex = 1;
    for (IKPSolution &ekpSolution : solutions) {
        int sumOfWeights = 0;
        vector<int> vars = ekpSolution.getVariables();
        for (unsigned int i = 0; i < vars.size(); i += 2) {
            sumOfWeights += vars[i];
        }
        // Computing the capacity of the KP Instance Solution
        int capacity =
            (int)(((float)instanceIndex / (float)(maxInstances + 1)) *
                  sumOfWeights);

        ekpSolution.setCapacity(capacity);
        instanceIndex++;
    }
}

/**
 * @brief Method applied after evaluation the population of solutions
 *
 * @param solutions
 */
void IKPProblem::afterEvaluation(vector<IKPSolution> &solutions) {
    // Computes the features of the instance
    // For the KP are:
    // Q, Max_P, Max_W, Min_P, Min_W, avg_eff, mean and std
    for (IKPSolution &solution : solutions) {
        vector vars = solution.getVariables();
        float maxW = 0;
        float maxP = 0;
        float minW = std::numeric_limits<float>::max();
        float minP = std::numeric_limits<float>::max();
        float avgEff = 0.0f;
        float solMEan = mean(vars);
        float std = standardDev(solMEan, vars);
        for (unsigned int i = 0; i < vars.size(); i++) {
            if (i % 2 == 0) {  // Weight
                if (vars[i] > maxW) {
                    maxW = vars[i];
                } else if (vars[i] < minW) {
                    minW = vars[i];
                }
                // Computes the avgEff
                if (i != vars.size() - 1) {
                    avgEff += vars[i + 1] / vars[i];
                }
            } else {  // Profit
                if (vars[i] > maxP) {
                    maxP = vars[i];
                } else if (vars[i] < minP) {
                    minP = vars[i];
                }
            }
        }
        avgEff /= solution.getNumberOfVars();
        vector<float> features = {(float)solution.getCapacity(),
                                  maxP,
                                  maxW,
                                  minP,
                                  minW,
                                  avgEff,
                                  solMEan,
                                  std};
        solution.setFeatures(features);
    }
}
