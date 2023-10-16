/**
 * @file AbstractEA.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_ABSTRACTEA_H
#define DIGNEA_ABSTRACTEA_H

#include <dignea/core/Front.h>
#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>
#include <dignea/evaluators/PopulationEvaluator.h>
#include <dignea/evolution/AvgEvolution.h>
#include <dignea/evolution/Evolution.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/exceptions/NotImplemented.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <utility>

using json = nlohmann::json;

/**
 * @brief Class to define an Abstract Evolutionary Algorithm.
 * This is the base skeleton for future extensions.
 *
 * @tparam S Type of individuals
 */
template <class S>
class AbstractEA {
   public:
    AbstractEA();

    AbstractEA(const int &maxEvals, const int &popsize);

    AbstractEA(unique_ptr<PopulationEvaluator<S>>, const int &maxEvals,
               const int &popsize);

    virtual ~AbstractEA();

    /**
     * @brief Main method of the EA. Runs the algorithm but must be implemented
     * in the subclasses.
     *
     */
    virtual void run() = 0;

    /**
     * @brief Returns the name of the algorithm, this is used in the to_json
     * method. Must be implemented in the subclasses.
     *
     * @return string
     */
    virtual string getName() const = 0;

    /**
     * @brief Returns the identificator of the algorithm, this is used in the
     * to_json method. Must be implemented in the subclasses. It should return
     * the acronym of useful id for a particular configuration.
     *
     * @return string
     */
    virtual string getID() const = 0;

    virtual json to_json() const;

    /**
     * @brief Returns a Front object with all the solutions of the evolutionary
     * process. This method must be implemented in the subclasses.
     *
     * @return Front<S>
     */
    virtual Front<S> getResults() const = 0;

    /**
     * @brief Get the population size
     *
     * @return int
     */
    inline int getPopulationSize() const { return populationSize; };

    void setPopulationSize(int pSize);

    /**
     * @brief Get the elapsed time of the evolutionary process
     *
     * @return double
     */
    inline double getElapsedTime() const { return elapsedTime; };

    /**
     * @brief Gets a reference of the population
     *
     * @return const vector<S>&
     */
    inline const vector<S> &getPopulation() const { return population; };

    /**
     * @brief Get the maximum number of evaluations to perform
     *
     * @return int
     */
    int getMaxEvaluations() const { return maxEvaluations; }

    /**
     * @brief Set a new maximum number of evaluations to perform
     *
     * @param maxEval
     */
    void setMaxEvaluations(int maxEval) {
        AbstractEA::maxEvaluations = maxEval;
    }

    /**
     * @brief Gets a pointer to the problem which is being solved
     *
     * @return const Problem<S>*
     */
    const Problem<S> *getProblem() const { return problem.get(); }

    /**
     * @brief Set the new problem to solve. Uses a shared_ptr pointer which
     * updates the reference counter. Does no take the ownership.
     *
     * @param prob
     */
    virtual void setProblem(shared_ptr<Problem<S>> prob) {
        this->problem = prob;
    }

    /**
     * @brief Set the new problem to solve. Uses a raw pointer and takes the
     * ownership of the object.
     *
     * @param prob
     */
    virtual void setProblem(Problem<S> *prob) { this->problem.reset(prob); }

    /**
     * @brief Gets a pointer to the population evaluator system
     *
     * @return PopulationEvaluator<S>*
     */
    PopulationEvaluator<S> *getEvaluator() const { return evaluator.get(); }

    /**
     * @brief Updates the population evaluator system with the unique_ptr. The
     * method takes the ownership of the object behind.
     *
     * @param eval
     */
    void setEvaluator(unique_ptr<PopulationEvaluator<S>> eval) {
        this->evaluator = move(eval);
    }

    /**
     * @brief Get the performed evaluations
     *
     * @return int
     */
    int getPerformedEvaluations() const { return performedEvaluations; }

    /**
     * @brief Set the Performed Evaluations. Useful for the updateProgress
     * method.
     *
     * @param pEvals
     */
    void setPerformedEvaluations(int pEvals) {
        this->performedEvaluations = pEvals;
    }

    /**
     * @brief Set the Population
     *
     * @param pop
     */
    void setPopulation(const vector<S> &pop);

    /**
     * @brief Get the Evolution data
     *
     * @return Evolution<S>
     */
    virtual Evolution<S> getEvolution() const;

    /**
     * @brief Get the interval of checkpoints
     *
     * @return int
     */
    int getPrintingInterval() const { return evolutionInterval; }

   protected:
    /**
     * @brief Initialises the evolutionary progress. This method must be
     * implemented in the subclasses and should perform all necessary steps
     * before the main loop.
     *
     */
    virtual void initProgress() = 0;

    /**
     * @brief Method which updates the evolutionary progress. This method must
     * be implemented in the subclasses and it should perform thinks like
     * updating the number of performed evaluations, etc.
     *
     */
    virtual void updateProgress() = 0;

    /**
     * @brief Method which finishes the evolutionary progress. This method must
     * be implemented in the subclasses and it should perform thinks like
     * computing the execution time, etc.
     *
     */
    virtual void finishProgress() = 0;

    /**
     * @brief Check whether the number of performed evaluations has reached the
     * maximum allowed. Must be implemented in the subclasses to adapt special
     * cases.
     *
     * @return true if the maximum number of evaluations have been performed
     * @return false otherwise
     */
    virtual bool isStoppingConditionReached() = 0;

    /**
     * @brief Creates the initial population of individuals. Must be implemented
     * in the subclasses to adapt special cases.
     */
    virtual void createInitialPopulation() = 0;

    /**
     * @brief Evaluates the entire population of solutions. This is a virtual
     * method that must be implemented in the subclasses to adapt special cases.
     *
     */
    virtual void evaluatePopulation(vector<S> &pop) = 0;

    virtual void updateEvolution(vector<S> &pop);

    virtual void updateEvolution(const int &checkpoint, vector<S> &);

   protected:
    int maxEvaluations; /*!< Number of evaluations to perform by the EA */
    int performedEvaluations;
    int populationSize; /*!< Number of individuals in the population */

    // Population of individuals --> Solutions
    vector<S> population;
    // Problem to solve
    shared_ptr<Problem<S>> problem;
    // Evaluation class
    unique_ptr<PopulationEvaluator<S>> evaluator;
    // Evolution data
    Evolution<S> evolution;
    AvgEvolution<S> avgEvolution;

    // Execution time
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    double elapsedTime;

    // Checkpoints to collect evolution data
    int nextCheckpoint;
    int evolutionInterval;

   public:
    static const int DEFAULT_POPULATION_SIZE;
    static const int DEFAULT_EVALUATIONS_LIMIT;
    static const std::string NAME;
    static const std::string MAX_EVALUATIONS;
    static const std::string POP_SIZE;
    static const std::string ELAPSED_TIME;
    static const std::string EVALUATOR;
    static const int EVOLUTION_SIZE;
};

/**
 * @brief Default population size equal to 100 individuals
 *
 * @tparam S
 */
template <class S>
const int AbstractEA<S>::DEFAULT_POPULATION_SIZE = 100;
/**
 * @brief Default evaluation limit equal to 100000 evaluations
 *
 * @tparam S
 */
template <class S>
const int AbstractEA<S>::DEFAULT_EVALUATIONS_LIMIT = 100000;
template <class S>
const std::string AbstractEA<S>::NAME = "Algorithm";
template <class S>
const std::string AbstractEA<S>::MAX_EVALUATIONS = "Max Evaluations";
template <class S>
const std::string AbstractEA<S>::POP_SIZE = "Population Size";
template <class S>
const std::string AbstractEA<S>::ELAPSED_TIME = "Elapsed Time";
template <class S>
const std::string AbstractEA<S>::EVALUATOR = "Evaluator";
template <class S>
const int AbstractEA<S>::EVOLUTION_SIZE = 10;

/**
 * @brief Construct a new Abstract EA object with default
 * parameter values.
 * - maxEvaluations is set to 100000 evaluations.
 * - populationSize is set to 100 individuals of type S.
 *
 * @tparam S
 */
template <class S>
AbstractEA<S>::AbstractEA()
    : maxEvaluations(DEFAULT_EVALUATIONS_LIMIT),
      performedEvaluations(0),
      populationSize(DEFAULT_POPULATION_SIZE),
      elapsedTime(0.0),
      nextCheckpoint(0) {
    evolution = {};
    avgEvolution = {};
    evolutionInterval = maxEvaluations / EVOLUTION_SIZE;
}

/**
 * @brief AbstractEA constructor which initialises the parameters maxEvalautions
 * and populationSize.
 *
 * @tparam S
 * @param maxEvals Maximum number of evaluations to perform
 * @param popsize Number of individuals in the population
 */
template <class S>
AbstractEA<S>::AbstractEA(const int &maxEvals, const int &popsize)
    : maxEvaluations(maxEvals),
      performedEvaluations(0),
      populationSize(popsize),
      elapsedTime(0.0),
      nextCheckpoint(0) {
    evolutionInterval = maxEvaluations / EVOLUTION_SIZE;
    problem = nullptr;
    this->evaluator = make_unique<PopulationEvaluator<S>>();
    evolution = {};
    avgEvolution = {};
}

/**
 * @brief AbstractEA constructor which initialises the parameters
 * maxEvalautions, the populationSize and the evaluator approach.
 * @tparam S
 * @param eval Evaluator approach (Sequential, parallel, OMP, etc)
 * @param maxEvals Maximum number of evaluations to perform
 * @param popsize Number of individuals in the population
 */
template <class S>
AbstractEA<S>::AbstractEA(unique_ptr<PopulationEvaluator<S>> eval,
                          const int &maxEvals, const int &popsize)
    : maxEvaluations(maxEvals),
      performedEvaluations(0),
      populationSize(popsize),
      elapsedTime(0.0),
      nextCheckpoint(0) {
    problem = nullptr;
    evaluator = move(eval);
    evolution = {};
    avgEvolution = {};
    evolutionInterval = maxEvaluations / EVOLUTION_SIZE;
}

template <class S>
AbstractEA<S>::~AbstractEA() {
    population.clear();
    population.shrink_to_fit();
}

/**
 * Updates the evolution results of the algorithm for the given checkpoint
 * @tparam Problem
 * @tparam S
 * @param solutions Population of solutions in the checkpoin to collect data
 * from
 */
template <class S>
void AbstractEA<S>::updateEvolution(vector<S> &solutions) {
    if (nextCheckpoint > maxEvaluations) {
        string where = "nextCheckpoint > maxEvals(" +
                       to_string(maxEvaluations) +
                       ") in AbstractEA::updateEvolution with: " +
                       to_string(nextCheckpoint) + " evals";
        throw(OutOfRange(where));
    }
    if (performedEvaluations >= nextCheckpoint) {
        evolution.update(nextCheckpoint, solutions);
        avgEvolution.update(nextCheckpoint, solutions);
        nextCheckpoint += evolutionInterval;
    }
}

/**
 * Updates the evolution results of the algorithm for the given checkpoint
 * @tparam Problem
 * @tparam S
 * @param checkpoint Checkpoint in the evolution process
 * @param solutions solutions Population of solutions in the checkpoin to
 * collect data from
 */
template <class S>
void AbstractEA<S>::updateEvolution(const int &checkpoint,
                                    vector<S> &solutions) {
    if (checkpoint > (maxEvaluations + evolutionInterval)) {
        string where =
            "checkpoint > maxEvals in AbstractEA::updateEvolution with: " +
            to_string(checkpoint) +
            " evals and maxEvals: " + to_string(maxEvaluations);
        throw(OutOfRange(where));
    }
    if (checkpoint >= nextCheckpoint) {
        evolution.update(checkpoint, solutions);
        avgEvolution.update(nextCheckpoint, solutions);
        nextCheckpoint += evolutionInterval;
    }
}

/**
 * @brief Returns the data of the evolution process. Includes the best fitness
 * obtained at each evaluated checkpoint.
 *
 * @tparam S
 * @return Evolution<S>
 */
template <class S>
Evolution<S> AbstractEA<S>::getEvolution() const {
    return this->evolution;
}

/**
 * @brief Setter to update the population size.
 *
 * @tparam S
 * @param pSize New size of the population
 */
template <class S>
void AbstractEA<S>::setPopulationSize(int pSize) {
    this->populationSize = pSize;
    this->population.clear();
    this->population.reserve(populationSize);
}

/**
 * @brief Set the population of the EA.
 *
 * @param pop New population to the EA.
 */
template <class S>
void AbstractEA<S>::setPopulation(const vector<S> &pop) {
    this->population = pop;
    this->populationSize = pop.size();
}
/**
 * @brief Generates and returns the JSON representation of the EA
 *
 * @tparam S
 * @return json
 */
template <class S>
json AbstractEA<S>::to_json() const {
    json info;
    Evolution copy = this->evolution;
    AvgEvolution avgCopy = this->avgEvolution;
    info["name"] = this->getName();
    info["max_evals"] = this->maxEvaluations;
    info["pop_size"] = this->populationSize;
    info["elapsed_time"] = this->elapsedTime;
    info["evaluator"] = this->evaluator->getName();
    info["evolution"] = copy.results();
    info["avg_evolution"] = avgCopy.results();
    return info;
};

#endif  // DIGNEA_ABSTRACTEA_H
