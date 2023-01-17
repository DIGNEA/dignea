/**
 * @file AbstractGA.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date  2/12/20.
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_ABSTRACTGA_H
#define DIGNEA_ABSTRACTGA_H

#include <dignea/core/Crossover.h>
#include <dignea/core/Mutation.h>
#include <dignea/core/Problem.h>
#include <dignea/core/Replacement.h>
#include <dignea/core/Selection.h>
#include <dignea/evaluators/PopulationEvaluator.h>
#include <dignea/utilities/exceptions/NoMOAllowed.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <memory>
#include <vector>

#include "AbstractSolver.h"

using namespace std;

/**
 * @brief Class to represent an Abstract Genetic Algorithm.
 * Base skeleton is defined here, to extend in particular versions of GAs.
 * This class extends AbstractSolver. Use this class to create your own version of
 * GAs.
 * @tparam S Type of individual in the population.
 */
template <class S>
class AbstractGA : public AbstractSolver<S> {
   public:
    AbstractGA();

    virtual ~AbstractGA() = default;

    virtual string getName() const = 0;

    json to_json() const override;

    virtual Front<S> getResults() const;

    void run() override;

    void setProblem(shared_ptr<Problem<S>> prob) override;

    void setProblem(Problem<S> *prob) override;

   protected:
    virtual void initProgress();

    virtual void updateProgress() = 0;

    virtual void finishProgress();

    virtual bool isStoppingConditionReached();

    virtual void createInitialPopulation();

    virtual void evaluatePopulation(vector<S> &pop);

    virtual vector<S> createMating();

    virtual void reproduction(S &, S &);

   public:
    double getMutationRate() const;

    void setMutationRate(double mutationRate);

    double getCrossRate() const;

    void setCrossRate(double crossRate);

    const Mutation<S> *getMutation() const;

    void setMutation(unique_ptr<Mutation<S>> mutation);

    const Crossover<S> *getCrossover() const;

    void setCrossover(unique_ptr<Crossover<S>> crossover);

    const Selection<S> *getSelection() const;

    void setSelection(unique_ptr<Selection<S>> selectionOperator);

   protected:
    double mutationRate; /*!< Mutation rate to apply the mutation operator */
    double crossRate;    /*!< Crossover rate to apply the crossover operator */

    unique_ptr<Mutation<S>> mutation;       /*!< Mutation  operator */
    unique_ptr<Crossover<S>> crossover;     /*!< Crossover operator */
    unique_ptr<Selection<S>> selection;     /*!< Selection operator */
    unique_ptr<Replacement<S>> replacement; /*!< Replacement operator */

   public:
    static double DEFAULT_MUTATION_RATE;
    static double DEFAULT_CROSSOVER_RATE;
    static int DEFAULT_POPULATION_SIZE;
};

/**
 * @brief Default mutation rate for GAs set to 0.05.
 *
 * @tparam S
 */
template <class S>
double AbstractGA<S>::DEFAULT_MUTATION_RATE = 0.05;
/**
 * @brief Default crossover rate to GAs set to 0.8.
 *
 * @tparam S
 */
template <class S>
double AbstractGA<S>::DEFAULT_CROSSOVER_RATE = 0.8;

/**
 * @brief Default population size to GAs set to 32 individuals.
 *
 * @tparam S
 */
template <class S>
int AbstractGA<S>::DEFAULT_POPULATION_SIZE = 32;

/**
 * @brief Creates a new AbstractGA which initialises all parameter to default.
 * The operators will be set to null.
 *
 * @tparam S
 */
template <class S>
AbstractGA<S>::AbstractGA()
    : AbstractSolver<S>(),
      mutationRate(DEFAULT_MUTATION_RATE),
      crossRate(DEFAULT_CROSSOVER_RATE) {
    this->populationSize = DEFAULT_POPULATION_SIZE;
    this->mutation = nullptr;
    this->crossover = nullptr;
    this->selection = nullptr;
    this->evaluator = make_unique<PopulationEvaluator<S>>();
}

/**
 * @brief Creates the initial population of individuals before starting the
 * evolutionary process
 *
 * @tparam S
 */
template <class S>
void AbstractGA<S>::createInitialPopulation() {
    this->population.resize(this->populationSize);
    for (int i = 0; i < this->getPopulationSize(); i++) {
        this->population[i] = this->problem->createSolution();
    }
    // Usamos el método por si empleamos un evaluador paralelo
    this->evaluatePopulation(this->population);
    // Reseteamos el printingcheck --> Para el uso de repeticiones en los
    // experimentos
    this->nextCheckpoint = 0;
    this->updateEvolution(0, this->population);
    this->initProgress();
}

/**
 * @brief Evaluates the entire population of individuals using the problem
 * evaluation function
 *
 * @tparam S
 * @param pop
 */
template <class S>
void AbstractGA<S>::evaluatePopulation(vector<S> &pop) {
    this->evaluator->evaluate(pop, this->problem.get());
}

/**
 * @brief Checks whether the evolutionary process has reached the maximum limit
 *
 * @tparam S
 * @return true If the evolutionary has reached the maximum number of
 * evaluations to perform
 * @return false otherwise
 */
template <class S>
bool AbstractGA<S>::isStoppingConditionReached() {
    return (this->performedEvaluations >= this->maxEvaluations);
}

/**
 * @brief Starts the evolutionary process. By default this methods sets the
 * number of performed evaluations to the population size since it considers
 * that all individuals have been evaluated in the createInitialPopulation
 * method.
 *
 * @tparam S
 */
template <class S>
void AbstractGA<S>::initProgress() {
    this->performedEvaluations = this->populationSize;
}

/**
 * @brief Runs the evolutionary process. This is the main EA method.
 * This methods is in charge of:
 * 1. Creating the initial population.
 * 2. While the stop condition is not met:
 *      1. Mate the individuals.
 *      2. Evaluate the population.
 *      3. Perform the replacement.
 *      4. Update the process.
 *      5. Update the evolution metrics.
 *
 * @tparam S
 */
template <class S>
void AbstractGA<S>::run() {
    if (this->problem) {
        this->createInitialPopulation();
        this->startTime = chrono::system_clock::now();
        while (!this->isStoppingConditionReached()) {
            vector mating = this->createMating();
            this->evaluatePopulation(mating);
            this->population = replacement->replace(this->population, mating);
            this->updateProgress();
            this->updateEvolution(this->performedEvaluations, this->population);
        }
        this->endTime = chrono::system_clock::now();
        this->finishProgress();
    } else {
        throw std::runtime_error("Problem is not set in GA::run");
    }
}

/**
 * @brief Generates the mating population of individuals to be evaluated. The
 * individuals are selected and after that the genetic operators are applied
 * here.
 *
 * @tparam S
 * @return vector<S>
 */
template <class S>
vector<S> AbstractGA<S>::createMating() {
    vector<S> offspring(this->populationSize);
    for (int i = 0; i < this->populationSize; i++) {
        S child1 = selection->select(this->population);
        S child2 = selection->select(this->population);
        reproduction(child1, child2);
        offspring[i] = child1;
    }
    return offspring;
}

/**
 * @brief Applies the genetic operator. This methods is invoked from
 * createMating.
 *
 * @tparam S
 * @param child1
 * @param child2
 */
template <class S>
void AbstractGA<S>::reproduction(S &child1, S &child2) {
    // Cruzamos los individuos con cierta probabilidad
    if (PseudoRandom::randDouble() < crossRate) {
        crossover->run(child1, child2);
    }
    mutation->run(child1, mutationRate, this->problem.get());
}

/**
 * @brief Gets the mutation rate
 *
 * @tparam S
 * @return double
 */
template <class S>
double AbstractGA<S>::getMutationRate() const {
    return mutationRate;
}

/**
 * @brief Updates the mutation rate
 *
 * @tparam S
 * @param mutationRate
 */
template <class S>
void AbstractGA<S>::setMutationRate(double mutationRate) {
    AbstractGA::mutationRate = mutationRate;
}

/**
 * @brief Gets the crossover rate
 *
 * @tparam S
 * @return double
 */
template <class S>
double AbstractGA<S>::getCrossRate() const {
    return crossRate;
}

/**
 * @brief Updates the crossover rate
 *
 * @tparam S
 * @param crossRate
 */
template <class S>
void AbstractGA<S>::setCrossRate(double crossRate) {
    AbstractGA::crossRate = crossRate;
}

/**
 * @brief Gets a pointer to the mutation operator
 *
 * @tparam S
 * @return const Mutation<S>*
 */
template <class S>
const Mutation<S> *AbstractGA<S>::getMutation() const {
    return mutation.get();
}

/**
 * @brief Updates the mutation operator. Takes a unique_ptr pointing to the new
 * Mutation operator and takes the ownership.
 *
 * @tparam S
 * @param newMut
 */
template <class S>
void AbstractGA<S>::setMutation(unique_ptr<Mutation<S>> newMut) {
    this->mutation = move(newMut);
}

/**
 * @brief Returns a raw pointer to the crossover operator.
 *
 * @tparam S
 * @return const Crossover<S>*
 */
template <class S>
const Crossover<S> *AbstractGA<S>::getCrossover() const {
    return crossover.get();
}

/**
 * @brief Updates the crossover operator. Takes a unique_ptr pointing to the new
 * Crossover operator and takes the ownership.
 *
 * @tparam S
 * @param newCX
 */
template <class S>
void AbstractGA<S>::setCrossover(unique_ptr<Crossover<S>> newCX) {
    this->crossover = move(newCX);
}

/**
 * @brief Returns a raw pointer of the selection operator.
 *
 * @tparam S
 * @return const Selection<S>*
 */
template <class S>
const Selection<S> *AbstractGA<S>::getSelection() const {
    return selection.get();
}

/**
 * @brief Updates the selection operator. Takes a unique_ptr pointing to the new
 * Selection operator and takes the ownership.
 *
 * @tparam S
 * @param newSelection
 */
template <class S>
void AbstractGA<S>::setSelection(unique_ptr<Selection<S>> newSelection) {
    this->selection = move(newSelection);
}

/**
 * @brief Uptades the problem to solve using a share_ptr. The problem must be
 * single-objective.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void AbstractGA<S>::setProblem(shared_ptr<Problem<S>> prob) {
    if (prob->getNumberOfObjs() != 1) {
        std::string where = "AbstractGA setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem = prob;
}
/**
 * @brief Uptades the problem to solve using a raw pointer. The problem must be
 * single-objective.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void AbstractGA<S>::setProblem(Problem<S> *prob) {
    if (prob->getNumberOfObjs() != 1) {
        std::string where = "AbstractGA setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem.reset(prob);
}

/**
 * @brief Finishes the evolutionary process by computing the elapsed time.
 *
 * @tparam S
 */
template <class S>
void AbstractGA<S>::finishProgress() {
    std::chrono::duration<double> diff = this->endTime - this->startTime;
    this->elapsedTime = diff.count();
}

/**
 * @brief Returns a front with the feasible non repeated individuals in the last
 * population of the Genetic Algorithm. This is the final solution of the
 * algorithm to the problem at hand.
 *
 * @tparam S
 * @return Front<S>
 */
template <class S>
Front<S> AbstractGA<S>::getResults() const {
    if (!this->population.empty()) {
        vector<S> results;
        // Incluimos solo los individuos factibles
        for (const S &individual : this->population) {
            if (individual.getConstraintCoeff() == 0.0) {
                results.push_back(S(individual));
            }
        }
        results.erase(unique(results.begin(), results.end()), results.end());
        return Front<S>(results);
    } else
        return Front<S>(0);
}

/**
 * @brief Creates and returns JSON object with the GA information
 * @tparam S
 * @return a json representation of the GA
 */
template <class S>
json AbstractGA<S>::to_json() const {
    json data = AbstractSolver<S>::to_json();
    if (mutation) data["mutation"] = this->mutation->getName();
    if (crossover) data["crossover"] = this->crossover->getName();
    if (selection) data["selection"] = this->selection->getName();
    if (replacement) data["replacement"] = this->replacement->getName();
    data["mutation_rate"] = this->mutationRate;
    data["crossover_rate"] = this->crossRate;
    return data;
}

#endif  // DIGNEA_ABSTRACTGA_H
