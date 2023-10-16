/**
 * @file NSGA-II.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_NSGA2_H
#define DIGNEA_NSGA2_H

#include <dignea/core/AbstractGA.h>
#include <dignea/core/Crossover.h>
#include <dignea/core/Mutation.h>
#include <dignea/core/Solution.h>
#include <dignea/utilities/Comparator.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <ranges>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Class to represents a Non-dominated Sorting Genetic Algorithm II.
 *   -----------------------------------------------------
 *   NSGA: Non-dominated Sorting GA (Srinivas & Deb, 1995)
 *   -----------------------------------------------------
 *   -> N. Srinivas, K. Deb:
 *      Multiobjetive Optimization Using Nondominated Sorting in Genetic
 * Algorithms. Evolutionary Computation 2 (1995) 221-248.
 *
 *   -----------------------------------------------------
 *   NSGA-II: Non-dominated Sorting Genetic Algorithm II
 *   -----------------------------------------------------
 *   The algorithm was proposed by Deb in 2000. It is based on:
 *   - A fast non-dominated sorting approach, based on dominance fronts
 *   - A crowding operator
 *   - A selection operator which creates a mating pool by cominning the parent
 * and child populations and selecting the best with respect to fitness and
 * spread
 *
 * @tparam S Type of individual in the population
 */
template <class S>
class NSGA2 : public AbstractGA<S> {
   public:
    NSGA2();

    virtual ~NSGA2() = default;
    /**
     * @brief Get the Name
     *
     * @return string
     */
    string getName() const override {
        return "Non-dominated Sorting Genetic Algorithm II";
    }
    /**
     * @brief Get the ID of the algorithm
     *
     * @return string
     */
    string getID() const override { return "NSGA-II"; }

    void run() override;

    void setProblem(shared_ptr<Problem<S>> prob) override;

    void setProblem(Problem<S> *prob) override;

    virtual Front<S> getResults() const;

   protected:
    void initProgress() override;

    void updateProgress() override;

    virtual void createInitialPopulation() override;

    virtual void evaluatePopulation(vector<S> &pop) override;

    virtual vector<S> createMating() override;

    virtual void reproduction(S &, S &) override;

   private:
    void rankPopulation();
    vector<S> rankCrowdingPopulation(vector<S> &combinedPopulation);
};

/**
 * @brief Creates a RAW instance of a Generational GA algorithm
 * @tparam Problem
 * @tparam S
 */
template <class S>
NSGA2<S>::NSGA2() : AbstractGA<S>() {}

/**
 * @brief Creates the initial population of individuals before starting the
 * evolutionary process
 *
 * @tparam S
 */
template <class S>
void NSGA2<S>::createInitialPopulation() {
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
 * @brief Starts the evolutionary process. By default this methods sets the
 * number of performed evaluations to the population size since it considers
 * that all individuals have been evaluated in the createInitialPopulation
 * method.
 *
 * @tparam S
 */
template <class S>
void NSGA2<S>::initProgress() {
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
void NSGA2<S>::run() {
    if (this->problem) {
        this->createInitialPopulation();
        this->startTime = chrono::system_clock::now();
        this->rankPopulation();
        while (!this->isStoppingConditionReached()) {
            vector offspring = this->createMating();

            offspring.insert(offspring.end(), this->population.begin(),
                             this->population.end());
            this->population = this->rankCrowdingPopulation(offspring);
            this->updateEvolution(this->performedEvaluations, this->population);
        }
        this->endTime = chrono::system_clock::now();
        this->finishProgress();
    } else {
        throw std::runtime_error("Problem is not set in GA::run");
    }
}

/**
 * @brief TODO
 *
 * @tparam S
 */
template <class S>
void NSGA2<S>::rankPopulation() {
    vector<vector<S>> fronts =
        fastNonDominatedSort(this->population, this->problem.get());
    this->population.clear();
    for (vector<S> front : fronts) {
        for (S solution : front) {
            this->population.push_back(solution);
        }
    }
}

/**
 * @brief Create fronts using the non-domination rank until pSize elements have
 * been selected.
 * Each front is ordered using the local crowding distance.
 * In: population without any order with size = pSize * 2
 * Exit: population ordered using the crowded comparison operator with size
 * pSize
 *
 *
 * @tparam S
 */
template <class S>
vector<S> NSGA2<S>::rankCrowdingPopulation(vector<S> &combinedPopulation) {
    vector<vector<S>> fronts =
        fastNonDominatedSort(combinedPopulation, this->problem.get());
    vector<S> newPopulation;
    newPopulation.reserve(this->populationSize);
    auto toInsert = this->populationSize;
    auto index = 0;
    // We sort the front using the crowding operator
    // Then we include each solution in the new population
    std::for_each(fronts.begin(), fronts.end(),
                  [&](vector<S> &f) { crowOrder(f, this->problem.get()); });
    // Include all solutions in the fronts that fits perfectly
    bool fits = toInsert <= (int)fronts[0].size();
    while (fits) {
        for (S solution : fronts[index]) {
            newPopulation.push_back(solution);
            toInsert--;
            if (toInsert == 0) {
                break;
            }
        }
        index++;
        if ((toInsert <= 0) || (toInsert <= (int)fronts[index].size())) {
            fits = false;
        }
    }
    // If we don't have enough solutions yet to create a new population
    // and the remaining fronts doesn't fit entirely, we include only
    // the `n` necessary new solutions
    if (toInsert > 0) {
        vector<vector<S>> remaining(fronts.begin() + index, fronts.end());
        auto jv = std::ranges::join_view(remaining);
        std::ranges::for_each(
            jv | std::ranges::views::take(toInsert),
            [&](const S &solution) { newPopulation.push_back(solution); });
    }
    return newPopulation;
}

/**
 * @brief Evaluates the entire population of individuals using the problem
 * evaluation function
 *
 * @tparam S
 * @param pop
 */
template <class S>
void NSGA2<S>::evaluatePopulation(vector<S> &pop) {
    this->evaluator->evaluate(pop, this->problem.get());
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
vector<S> NSGA2<S>::createMating() {
    // Generates offsprings
    vector<S> offspring;
    for (int i = 0; i < this->populationSize; i++) {
        S child1 = this->selection->select(this->population);
        S child2 = this->selection->select(this->population);
        reproduction(child1, child2);
        this->problem->evaluate(child1);
        this->problem->evaluate(child2);
        offspring.push_back(child1);
        offspring.push_back(child2);
        this->performedEvaluations += 2;
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
void NSGA2<S>::reproduction(S &child1, S &child2) {
    // Cruzamos los individuos con cierta probabilidad
    if (PseudoRandom::randDouble() < this->crossRate) {
        this->crossover->run(child1, child2);
    }
    this->mutation->run(child1, this->mutationRate, this->problem.get());
    this->mutation->run(child2, this->mutationRate, this->problem.get());
}

/**
 * @brief Updates the performed evaluations to the population size on each call.
 * @tparam Problem
 * @tparam S
 */
template <class S>
void NSGA2<S>::updateProgress() {
    this->performedEvaluations += 2;
}

/**
 * @brief Uptades the problem to solve using a share_ptr. The problem must be
 * multi-objective.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void NSGA2<S>::setProblem(shared_ptr<Problem<S>> prob) {
    if (prob->getNumberOfObjs() != 2) {
        std::string where = "NSGA2 setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem = prob;
}
/**
 * @brief Uptades the problem to solve using a raw pointer. The problem must be
 * multi-objective.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void NSGA2<S>::setProblem(Problem<S> *prob) {
    if (prob->getNumberOfObjs() != 2) {
        std::string where = "NSGA2 setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem.reset(prob);
}

/**
 * @brief Creates the front of solution from the evolution
 *
 * @tparam S
 * @return Front<S>
 */
template <class S>
Front<S> NSGA2<S>::getResults() const {
    Front<S> front(0);
    std::ranges::for_each(this->population, [&](const S &solution) {
        front.addSolution(solution, this->problem.get());
    });
    return front;
}

#endif  // DIGNEA_NSGA2_H
