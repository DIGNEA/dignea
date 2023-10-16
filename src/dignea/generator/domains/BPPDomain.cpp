/**
 * @file BPPDomain.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/generator/domains/BPPDomain.h>
#include <dignea/generator/instances/BPPInstance.h>
#include <dignea/utilities/Statistics.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <random>
#include <ranges>

using namespace std;

BPPDomain::BPPDomain()
    : AbstractDomain<BPP, BPPInstance>(1, 1, 0),
      maxWeight(1000),
      minWeight(0) {}

BPPDomain::BPPDomain(const int &numberOfVars, const int &numberOfObjs,
                     const int &capacity)

    : AbstractDomain<BPP, BPPInstance>(numberOfVars, numberOfObjs, 0),
      maxWeight(1000),
      minWeight(0),
      defaultCapacity(capacity) {}

BPPDomain::BPPDomain(const int &numberOfVars, const int &numberOfObjs,
                     const int &capacity, const int &minW, const int &maxW)

    : AbstractDomain<BPP, BPPInstance>(numberOfVars, numberOfObjs, 0),
      maxWeight(maxW),
      minWeight(minW),
      defaultCapacity(capacity) {}

/**
 * Method which evaluates a solution of the BPP Instance Generation Problem
 * @param solution
 */
void BPPDomain::evaluate(BPPInstance &solution) const { return; }

/**
 * Generates a new solution without args
 * @return
 */
BPPInstance BPPDomain::createSolution() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minWeight, maxWeight);
    auto generator = [&dist, &gen]() { return dist(gen); };

    vector items(this->numberOfVars, 0);
    std::generate(items.begin(), items.end(), generator);
    BPPInstance instance(this->numberOfVars, this->numberOfObjs);
    instance.setVariables(items);
    instance.setCapacity(this->defaultCapacity);

    return instance;
}

/**
 * Creates a population of solutions
 * @param maxSolutions
 * @return
 */
vector<BPPInstance> BPPDomain::createSolutions(const int &maxSolutions) const {
    vector<BPPInstance> solutions;
    solutions.reserve(maxSolutions);
    for (int i = 0; i < maxSolutions; i++) {
        solutions.push_back(this->createSolution());
    }
    return solutions;
}

BPPInstance BPPDomain::createSolution(ParallelPRNG &engine) const {
    return this->createSolution();
}

/**
 * Returns the upper bound for the variable i
 * @param i
 * @return
 */
float BPPDomain::getUpperLimit(const int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getUpperLimit in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return this->maxWeight;
}

/**
 * Returns the lower bound for the variable i
 * @param i
 * @return
 */
float BPPDomain::getLowerLimit(const int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getLowerLimit in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return this->minWeight;
}

/**
 * @brief Creates a Optimisation Problem for the Configurations on the EIG
 * In this case, BPP Problem
 * @return
 */
shared_ptr<BPP> BPPDomain::genOptProblem(const BPPInstance &instance) const {
    vector variables = instance.getVariables();
    if (variables.size() != (unsigned int)this->numberOfVars) {
        string where = "instance.variables.size() != Problem inner dimension";
        throw OutOfRange(where);
    }

    return make_shared<BPP>(this->numberOfVars, this->defaultCapacity,
                            variables);
}

/**
 * Method which returns the optimisation direction for each objective
 * Minimize
 * @param i
 * @return
 */
int BPPDomain::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= 1) {
        std::string where =
            "getOptimizationDirection in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

/**
 * Method which re-calculates the capacity of the instances before evaluate them
 * The capacity is set as the 80% of the sum of the weights of the items
 * @param vector1
 */
void BPPDomain::beforeEvaluation(vector<BPPInstance> &solutions) {
    // Nothing Really
    return;
}

/**
 * @brief Method applied after evaluation the population of solutions
 *
 * @param solutions
 */
void BPPDomain::afterEvaluation(vector<BPPInstance> &solutions) {
    // Computes the features of the instance
    // For the BPP the features are:
    // N, Capacity, MeanWeights, MedianWeights, VarianceWeights, MaxWeight,
    // MinWeight, Huge, Large, Medium, Small, Tiny
    for (BPPInstance &solution : solutions) {
        vector vars = solution.getVariables();
        auto n = vars.size();
        vector<float> normVars(n);
        for (unsigned int i = 0; i < n; i++) {
            normVars[i] = (float)vars[i] / (float)solution.getCapacity();
        }

        float meanW = mean(normVars);
        auto medianW = median(normVars);
        float std = standardDev(meanW, normVars);
        auto [min, max] = minmax_element(normVars.begin(), normVars.end());

        namespace ranges = std::ranges;
        float huge = (float)((float)ranges::count_if(
                                 normVars, [](float i) { return i > 0.5f; }) /
                             (float)n);
        float large =
            (float)((float)ranges::count_if(
                        normVars,
                        [](float i) { return (0.5f >= i && i > (1 / 3)); }) /
                    (float)n);
        float medium =
            (float)((float)ranges::count_if(
                        normVars,
                        [](float i) { return (i <= (1 / 3) && i > 0.25f); }) /
                    (float)n);
        float small =
            (float)((float)ranges::count_if(
                        normVars, [](float i) { return 0.25f >= i; }) /
                    (float)n);
        float tiny = (float)((float)ranges::count_if(
                                 normVars, [](float i) { return 0.1f >= i; }) /
                             (float)n);

        // Mean, Std, Median,
        // Max Weight, Min Weight, Small, Medium, Large, Huge
        vector<float> features = {
            meanW, std,    (float)medianW, (float)*max, (float)*min, tiny,
            small, medium, large,          huge

        };
        solution.setFeatures(features);
    }
}

json BPPDomain::to_json() const {
    json data = AbstractDomain<BPP, BPPInstance>::to_json();
    data["defaultCapacity"] = this->defaultCapacity;
    data["minWeight"] = this->minWeight;
    data["maxWeight"] = this->maxWeight;
    return data;
}
