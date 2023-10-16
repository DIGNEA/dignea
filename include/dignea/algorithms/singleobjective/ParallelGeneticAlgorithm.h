//
// Created by amarrero on 29/1/21.
//

#ifndef DIGNEA_PARALLELGENETICALGORITHM_H
#define DIGNEA_PARALLELGENETICALGORITHM_H

#include <dignea/core/AbstractGA.h>
#include <dignea/evaluators/ParallelOMPEvaluator.h>
#include <dignea/replacements/EGenerational.h>
#include <dignea/utilities/random/ParallelPRNG.h>
#include <omp.h>

#include <algorithm>
#include <iterator>
#include <ranges>
#include <vector>

using namespace std;

/**
 * @brief Class to represents a Parallel Genetic Algorithm (ParGA).
 * This algorithm runs the evolution in numberOfCores different cores
 * to speedup the evaluation.
 * Inherits from AbstractGA.
 * @tparam S Type of individual in the population
 */
template <class S>
class ParallelGeneticAlgorithm : public AbstractGA<S> {
   public:
    ParallelGeneticAlgorithm();

    virtual ~ParallelGeneticAlgorithm();
    /**
     * @brief Get the Name
     *
     * @return string
     */
    virtual string getName() const override {
        return "Parallel Genetic Algorithm";
    };

    virtual void run() override;
    /**
     * @brief Get the number of cores used
     *
     * @return int
     */
    int getNumberOfCores() const { return numberOfCores; }
    /**
     * @brief Set the number of cores to use
     *
     * @param nCores
     */
    void setNumberOfCores(int nCores) { this->numberOfCores = nCores; }

    void setProblem(shared_ptr<Problem<S>> prob) override;
    /**
     * @brief Get a raw pointer to the problem to solve
     *
     * @return const Problem<S>*
     */
    const Problem<S> *getProblem() const { return problem.get(); }

    json to_json() const override;

   protected:
    virtual void initProgress();

    virtual void updateProgress(){};

    void runEvolution();

    virtual void createInitialPopulation();

    virtual void reproduction(S &, S &);

    virtual void replacement(vector<S> &offsp){};

    string getID() const override { return "ParGA"; };

   protected:
    S parallelSelection(const int &init, const int &end);

    void configureEnv();

   protected:
    int numberOfCores; /*!< Number of cores to run in parallel */
    int chunks;        /*!< Chunks of population for each core */
    ParallelPRNG prng; /*!< Random number generator */

    vector<S> individuals;
    shared_ptr<Problem<S>> problem;
    vector<float> bestEvolution;
};

/**
 * @brief Creates a default ParallelGeneticAlgorithm. Use the ParGABuilder class
 * instead.
 * @tparam Problem
 * @tparam S
 */
template <class S>
ParallelGeneticAlgorithm<S>::ParallelGeneticAlgorithm() : AbstractGA<S>() {
    this->numberOfCores = 2;  // Lets set 2 cores by default
    this->chunks = 0;
    this->evaluator = make_unique<ParallelOMPEvaluator<S>>(numberOfCores);
}
/**
 * @brief Configures the parallel environment
 *
 * @tparam S
 */
template <class S>
void ParallelGeneticAlgorithm<S>::configureEnv() {
    this->individuals.resize(this->populationSize);
    this->performedEvaluations = 0;
    this->chunks = this->populationSize / this->numberOfCores;
    omp_set_num_threads(this->numberOfCores);
}

template <class S>
ParallelGeneticAlgorithm<S>::~ParallelGeneticAlgorithm() {}

/**
 * @brief Creates the initial population using parallel cores
 * @tparam V
 * @tparam O
 */
template <class S>
void ParallelGeneticAlgorithm<S>::createInitialPopulation() {
    const int popDim = this->populationSize;
    this->individuals.resize(popDim);
#pragma omp parallel for shared(problem) num_threads(numberOfCores)
    for (int i = 0; i < popDim; i++) {
        individuals[i] = problem->createSolution();
        problem->evaluate(individuals[i]);
    }
    this->nextCheckpoint = 0;
    this->initProgress();
}

/**
 * @brief Starts the progress of the parallel algorithm
 * Sets the performedEvaluations to the number of individuals in the population
 * @tparam Problem
 * @tparam S
 */
template <class S>
void ParallelGeneticAlgorithm<S>::initProgress() {
    this->performedEvaluations = this->populationSize;
}

/**
 * @brief Run the parallel Algorithm
 * - Starts and stops the timers
 * - Creates the initial population
 * - Run the evolution cycle
 * - Finishes the progress
 * @tparam Problem
 * @tparam S
 */
template <class S>
void ParallelGeneticAlgorithm<S>::run() {
    if (this->problem) {
        this->configureEnv();
        this->bestEvolution.clear();
        this->startTime = chrono::system_clock::now();
        this->createInitialPopulation();
        this->runEvolution();
        this->endTime = chrono::system_clock::now();
        this->finishProgress();
    } else {
        throw std::runtime_error("Problem is not set in ParallelGA::run");
    }
}

/**
 * @brief Main method of the ParallelGeneticAlgorithm.
 * Runs the evolution of the algorithm.
 * @tparam Problem
 * @tparam S
 */
template <class S>
void ParallelGeneticAlgorithm<S>::runEvolution() {
    const float eps = 1e-9;
    const int GENERATIONS = this->maxEvaluations / this->populationSize;
    int performedGenerations = 0;
    const int popDim = this->populationSize - 1;
    do {
        vector<S> offspring(this->populationSize);
#pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < this->populationSize; i++) {
            // Previously we splitted the population by cores
            // Splits thepopulation in chunks to perform the selection
            // int init = omp_get_thread_num() * chunks;
            // int end = init + (chunks - 1);
            int idx1 = min(prng.randInt(0, popDim), prng.randInt(0, popDim));
            int idx2 = min(prng.randInt(0, popDim), prng.randInt(0, popDim));
            S child1 = individuals[idx1];
            S child2 = individuals[idx2];
            this->reproduction(child1, child2);
            this->problem->evaluate(child1);
            // Replacement
            bool update = false;
            double childPenalty = child1.getConstraintCoeff();
            double individualPenalty = individuals[i].getConstraintCoeff();
            double childFitness = child1.getFitness();
            double individualFitness = individuals[i].getFitness();
            // If child has less penalty or in equal penalty values it has
            // better fitness
            if (childPenalty < individualPenalty) {
                update = true;
            } else if ((abs(childPenalty - individualPenalty) < eps) &&
                       (childFitness > individualFitness)) {
                update = true;
            }
            if (update) {
                offspring[i] = child1;
                // individuals[i] = child1;
            } else {
                offspring[i] = individuals[i];
            }
        }
// Replacement
#pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < this->populationSize; i++) {
            this->individuals[i] = offspring[i];
        }
#ifdef EVOLUTION
        // Get the best fitness in the generation
        auto it = std::ranges::max_element(individuals, {}, &S::getFitness);
        if (it != end(individuals)) {
            const auto pos = std::distance(begin(individuals), it);
            this->bestEvolution.push_back(individuals[pos].getFitness());
        }
#endif
        // Updating the performed evaluations
        performedGenerations++;
    } while (performedGenerations < GENERATIONS);
    // Updates superclass variables with this variables used for parallel
    // executions
    this->population = this->individuals;
    this->performedEvaluations = this->maxEvaluations;
}

/**
 * @brief Applies the genetic operators to the given individuals
 * @tparam Problem
 * @tparam S
 */
template <class S>
void ParallelGeneticAlgorithm<S>::reproduction(S &child1, S &child2) {
    if (prng.randDouble() < this->crossRate) {
        this->crossover->run(child1, child2);
    }
    this->mutation->run(child1, this->mutationRate, problem.get());
}

/**
 * @brief Parallel selection operator. This methods performs a binary tournament
 * selection in the range [int, end] of the population. Used for speedup the
 * parallel executions
 * @tparam Problem
 * @tparam S
 * @param init
 * @param end
 * @return
 */
template <class S>
S ParallelGeneticAlgorithm<S>::parallelSelection(const int &init,
                                                 const int &end) {
    int firstOption = prng.randInt(init, end);
    int secondOption = prng.randInt(init, end);
    S child = individuals[min(firstOption, secondOption)];
    return child;
}
/**
 * @brief Sets the problem to solve. Receives a share_ptr.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void ParallelGeneticAlgorithm<S>::setProblem(shared_ptr<Problem<S>> prob) {
    if (prob->getNumberOfObjs() != 1) {
        std::string where = "Parallel Genetic Algorithm::setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem = prob;
}
/**
 * @brief Generates and returns a JSON representation of the
 * ParallelGeneticAlgorithm
 *
 * @tparam S
 * @return json
 */
template <class S>
json ParallelGeneticAlgorithm<S>::to_json() const {
    json data = AbstractGA<S>::to_json();
    data["num_cores"] = this->numberOfCores;
#ifdef EVOLUTION
    data["evolution"] = this->bestEvolution;
#endif
    return data;
}

#endif  // DIGNEA_PARALLELGENETICALGORITHM_H