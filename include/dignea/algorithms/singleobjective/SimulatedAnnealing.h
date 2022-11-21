//
// Created by amarrero on 25/6/21.
//

#ifndef DIGNEA_SIMULATEDANNEALING_H
#define DIGNEA_SIMULATEDANNEALING_H

#include <dignea/core/AbstractEA.h>
#include <dignea/utilities/exceptions/NoMOAllowed.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <cmath>
#include <vector>

using namespace std;

/**
 * @brief Simulated Annealing Algorithm.
 *
 * @tparam S Type of individual in the population
 */
template <class S>
class SimulatedAnnealing : public AbstractEA<S> {
   public:
    SimulatedAnnealing(const int &maxEvaluations, const float &initialTemp,
                       const float &tempVariation);

    virtual ~SimulatedAnnealing() = default;

    void run() override;
    /**
     * @brief Get the Name
     *
     * @return string
     */
    string getName() const override { return "Simulated Annealing"; };
    /**
     * @brief Get the ID of the algorithm
     *
     * @return string
     */
    string getID() const override { return "SA"; }

    Front<S> getResults() const override;

    void setProblem(shared_ptr<Problem<S>> prob) override;

    void setProblem(Problem<S> *prob) override;
    /**
     * @brief Get the temperature variation constant for each generation
     *
     * @return float
     */
    float getTempVariation() const { return tempVariation; }
    /**
     * @brief Set the temperature variation constant for each generation
     *
     * @param tempVariation
     */
    void setTempVariation(float tempVariation) {
        SimulatedAnnealing::tempVariation = tempVariation;
    }
    /**
     * @brief Get the current temperature
     *
     * @return float
     */
    float getCurrentTemp() const { return currentTemp; }
    /**
     * @brief Set the current temperature of the process
     *
     * @param currentTemp
     */
    void setCurrentTemp(float currentTemp) {
        SimulatedAnnealing::currentTemp = currentTemp;
    }

    void setPopulationSize(const int &pSize);

   protected:
    void initProgress() override;

    void updateProgress() override;

    void finishProgress() override;

    bool isStoppingConditionReached() override;

    void createInitialPopulation() override;

    void evaluatePopulation(vector<S> &vector1) override{};

    S evaluateInds(S &popInd, S &offspring);

   private:
    S applyPerturbations();

    void updateTemp() { currentTemp *= tempVariation; }

   private:
    float tempVariation;
    float currentTemp;
};

/**
 * @brief Construct a new Simulated Annealing object with all the parameters.
 *
 * @tparam S
 * @param maxEvaluations
 * @param initialTemp
 * @param tempVariation
 */
template <class S>
SimulatedAnnealing<S>::SimulatedAnnealing(const int &maxEvaluations,
                                          const float &initialTemp,
                                          const float &tempVariation)
    : AbstractEA<S>(maxEvaluations, 1),
      tempVariation(tempVariation),
      currentTemp(initialTemp) {}

/**
 * @brief Runs the Simulated Annealing algorithm during maxEvaluations
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::run() {
    if (this->problem) {
        this->createInitialPopulation();
        this->startTime = chrono::system_clock::now();
        while (!this->isStoppingConditionReached()) {
            S offspring = applyPerturbations();
            this->population[0] = evaluateInds(this->population[0], offspring);
            updateTemp();
            this->updateProgress();
        }
        this->endTime = chrono::system_clock::now();
        this->finishProgress();
    } else {
        throw std::runtime_error("Problem is not set in SA::run");
    }
}

/**
 * @brief Returns the results obtained by the SA algorithm at the end of the
 * process.
 *
 * @tparam S
 * @return Front<S>
 */
template <class S>
Front<S> SimulatedAnnealing<S>::getResults() const {
    return Front<S>(this->population);
}

/**
 * @brief Initialises the evolutionary process. Sets the number of
 * performedEvaluations to 1.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::initProgress() {
    this->performedEvaluations = 1;
}

/**
 * @brief Updates the evolutionary process by increasing the
 * performedEvaluations in two.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::updateProgress() {
    this->performedEvaluations += 2;
}

/**
 * @brief Finishes the evolutionary process and computes the elapsed time.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::finishProgress() {
    std::chrono::duration<double> diff = this->endTime - this->startTime;
    this->elapsedTime = diff.count();
}

/**
 * @brief Checks whether the maximum number of evaluations have been performed
 *
 * @tparam S
 * @return true
 * @return false
 */
template <class S>
bool SimulatedAnnealing<S>::isStoppingConditionReached() {
    return (this->performedEvaluations >= this->maxEvaluations);
}

/**
 * @brief Creates the initial population for the SA. The population of the SA is
 * just one individual.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::createInitialPopulation() {
    this->population.resize(1);
    this->population[0] = this->problem->createSolution();
    this->evaluatePopulation(this->population);
    this->nextCheckpoint = 0;
    this->updateEvolution(0, this->population);
    this->initProgress();
}

/**
 * @brief Evaluates the parent and offspring individuals at each generation.
 *
 * @tparam S
 * @param popInd
 * @param offspring
 * @return S
 */
template <class S>
S SimulatedAnnealing<S>::evaluateInds(S &popInd, S &offspring) {
    this->evaluator->evaluate(popInd, this->problem.get());
    this->evaluator->evaluate(offspring, this->problem.get());
    double diff = offspring.getFitness() - popInd.getFitness();
    if (diff < 0.0) {
        return offspring;
    } else {
        const double randProb = PseudoRandom::randDouble();
        const double e = exp(-diff / this->currentTemp);
        const double prob = min(1.0, e);
        return (randProb < prob) ? offspring : popInd;
    }
}

/**
 * @brief Perturbation method of the SA algorithm. This method generates a new
 * offspring from the SA population (just one solution).
 *
 * @tparam S
 * @return S
 */
template <class S>
S SimulatedAnnealing<S>::applyPerturbations() {
    S offspring = this->population[0];
    double pert = PseudoRandom::randDouble();
    vector vars = offspring.getVariables();
    for (unsigned int i = 0; i < vars.size(); i++) {
        double newValue = pert * (this->problem->getUpperLimit(i) -
                                  this->problem->getLowerLimit(i)) +
                          this->problem->getLowerLimit(i);
        vars[i] = newValue;
    }
    offspring.setVariables(vars);
    return offspring;
}

/**
 * @brief Sets the problem to solve using Simulated Annealing.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void SimulatedAnnealing<S>::setProblem(shared_ptr<Problem<S>> prob) {
    if (prob->getNumberOfObjs() != 1) {
        std::string where = "SimulatedAnnealing setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem = prob;
}

/**
 * @brief Sets the problem to solve using Simulated Annealing.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void SimulatedAnnealing<S>::setProblem(Problem<S> *prob) {
    if (prob->getNumberOfObjs() != 1) {
        std::string where = "SimulatedAnnealing setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem.reset(prob);
}

/**
 * @brief This methods is not implemented for this class. The population size of
 * a SA algorithm is always 1.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void SimulatedAnnealing<S>::setPopulationSize(const int &pSize) {
    std::string where = "SimulatedAnnealing setPopulationSize";
    throw(NotImplemented(where));
}

#endif  // DIGNEA_SIMULATEDANNEALING_H
