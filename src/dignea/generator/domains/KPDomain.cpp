//
// Created by amarrero on 8/3/21.
//

#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/utilities/PCA.h>
#include <dignea/utilities/Statistics.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <ranges>

KPDomain::KPDomain()
    : AbstractDomain<KPNR, KPInstance>(1, 1, 0),
      numberOfInstances(-1),
      lowWeight(0),
      upWeight(0),
      lowProfit(0),
      upProfit(0),
      generatedInstances(0),
      reducedSpace(false) {}

KPDomain::KPDomain(const int &numberOfVars, const int &numberOfObjs)
    : AbstractDomain<KPNR, KPInstance>(numberOfVars, numberOfObjs, 0),
      numberOfInstances(-1),
      lowWeight(0),
      upWeight(0),
      lowProfit(0),
      upProfit(0),
      generatedInstances(0),
      reducedSpace(false) {}

KPDomain::~KPDomain() {}

KPDomain::KPDomain(const int &numberOfVars, const int &numberOfObjs,
                   const int &numOfInstances, const int &lowerWeight,
                   const int &upperWeight, const int &lowerProfit,
                   const int &upperProfit, bool reducedSpace)
    : AbstractDomain<KPNR, KPInstance>(numberOfVars, numberOfObjs, 0),
      numberOfInstances(numOfInstances),
      lowWeight(lowerWeight),
      upWeight(upperWeight),
      lowProfit(lowerProfit),
      upProfit(upperProfit),
      generatedInstances(0),
      reducedSpace(reducedSpace) {}

/**
 * Method which evaluates a solution of the Knapsack Instance Generation Problem
 * @param solution
 */
void KPDomain::evaluate(KPInstance &solution) const { return; }

/**
 * Creates a new solution for the Knapsack Instance Generation Problem
 * @param instanceIndex
 * @return
 */
KPInstance KPDomain::createSolution(const int &instanceIndex,
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
    KPInstance solution(this->numberOfVars, this->numberOfObjs,
                        this->reducedSpace);
    solution.setVariables(vars);
    solution.setCapacity(capacity);
    return solution;
}

/**
 * Generates a new solution without args
 * @return
 */
KPInstance KPDomain::createSolution() const {
    return this->createSolution(1, numberOfInstances);
}

KPInstance KPDomain::createSolution(ParallelPRNG &engine) const {
    return this->createSolution(1, numberOfInstances);
}

/**
 * Generates a new solution without args
 * @return
 */
KPInstance KPDomain::createSolution() {
    generatedInstances++;
    return this->createSolution(generatedInstances, numberOfInstances);
}

KPInstance KPDomain::createSolution(ParallelPRNG &engine) {
    generatedInstances++;
    return this->createSolution(generatedInstances, numberOfInstances);
}

/**
 * Returns the upper bound for the variable i
 * @param i
 * @return
 */
float KPDomain::getUpperLimit(const int i) const {
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
float KPDomain::getLowerLimit(const int i) const {
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
 * Creates a Optimisation Problem for the Configurations on the EIG
 * In this case, KPNR Problem
 * @return
 */
shared_ptr<KPNR> KPDomain::genOptProblem(const KPInstance &instance) const {
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
     * Unfolded loop which creates a KPNR problem from a KPInstance
     * The KPInstance stores the weights and profits in a single array
     * This loop creates two arrays from the one in the KPInstance object
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
int KPDomain::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= 2) {
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
vector<KPInstance> KPDomain::createSolutions(const int &maxSolutions) const {
    vector<KPInstance> solutions;
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
void KPDomain::beforeEvaluation(vector<KPInstance> &solutions) {
    // int maxInstances = solutions.size();
    // int instanceIndex = 1;
    for (KPInstance &ekpSolution : solutions) {
        int sumOfWeights = 0;
        vector<int> vars = ekpSolution.getVariables();
        for (unsigned int i = 0; i < vars.size(); i += 2) {
            sumOfWeights += vars[i];
        }
        // Computing the capacity of the KP Instance Solution
        int capacity = (int)(0.8 * sumOfWeights);

        ekpSolution.setCapacity(capacity);
        // instanceIndex++;
    }
}

/**
 * @brief Method applied after evaluation the population of solutions
 *
 * @param solutions
 */
void KPDomain::afterEvaluation(vector<KPInstance> &solutions) {
    // Computes the features of the instance
    // For the KP are:
    // Q, Max_P, Max_W, Min_P, Min_W, avg_eff, mean and std
    for (KPInstance &solution : solutions) {
        vector vars = solution.getVariables();
        float maxW = 0;
        float maxP = 0;
        float minW = std::numeric_limits<float>::max();
        float minP = std::numeric_limits<float>::max();
        float avgEff = 0.0f;
        float solMean = mean(vars);
        float std = standardDev(solMean, vars);
        vector<float> weights;
        vector<float> profits;
        weights.reserve(vars.size() / 2);
        profits.reserve(vars.size() / 2);

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
                // Include profits and weights to PCA
                weights.push_back((float)vars[i]);

            } else {  // Profit
                if (vars[i] > maxP) {
                    maxP = vars[i];
                } else if (vars[i] < minP) {
                    minP = vars[i];
                }
                profits.push_back((float)vars[i]);
            }
        }
        avgEff /= solution.getNumberOfVars();
        if (reducedSpace) {
            // We're testing the 6D to 2D space PCA using
            //"capacity", "min_p", "std", "mean", "avg_eff", "max_w"
            vector<float> highDimFeatures = {(float)solution.getCapacity(),
                                             minP,
                                             std,
                                             solMean,
                                             avgEff,
                                             maxW};
            // vector<float> highDimFeatures = {(float)solution.getCapacity(),
            //                                  avgEff,
            //                                  maxP,
            //                                  maxW,
            //                                  solMean,
            //                                  minP,
            //                                  minW,
            //                                  std};
            nc::NdArray<float> f = {highDimFeatures};
            auto coords = PCA::PCA(f);
            // Includes (x0, x1) principal components in the features
            vector<float> features = {
                coords[0], coords[1], (float)solution.getCapacity(),
                avgEff,    maxP,      maxW,
                solMean,   minP,      minW,
                std};
            solution.setFeatures(features);
        } else {
            // When we are not using a reduced space search
            // the NS was original fed with the features in this order
            vector<float> highDimFeatures = {(float)solution.getCapacity(),
                                             maxP,
                                             maxW,
                                             minP,
                                             minW,
                                             avgEff,
                                             solMean,
                                             std};
            solution.setFeatures(highDimFeatures);
        }
    }
}

json KPDomain::to_json() const {
    json data = AbstractDomain<KPNR, KPInstance>::to_json();
    data["isReducedSpace"] = this->reducedSpace;
    return data;
}
