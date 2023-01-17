/**
 * @file EIG.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 11/3/21.
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_EIG_H
#define DIGNEA_EIG_H

#include <dignea/core/AbstractSolver.h>
#include <dignea/core/Crossover.h>
#include <dignea/core/Mutation.h>
#include <dignea/core/Selection.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/distances/Euclidean.h>
#include <dignea/generator/AbstractDomain.h>
#include <dignea/generator/evaluations/EasyInstances.h>
#include <dignea/generator/evaluations/InstanceFitness.h>
#include <dignea/generator/evaluations/Weighted.h>
#include <dignea/metrics/AverageFitness.h>
#include <dignea/metrics/BestFitness.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/searches/NSFeatures.h>
#include <dignea/searches/NoveltySearch.h>
#include <dignea/selections/BinaryTournamentSelection.h>
#include <dignea/utilities/exceptions/Mismatch.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <numeric>
#include <vector>

using namespace std;

/**
 * @brief Instance Generation Algorithm. Known as Meta-Evolutionary Algorithm
 * (EIG). This algorithm uses a Weighted Fitness Function (WFF) to evaluate how
 * biased and diverse are the instances generated during the evolutionary
 * process. The instances generated should be biased to the performance of the
 * target algorithm in the portfolio but also should show some diversity between
 * them.
 *
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
class EIG : public AbstractSolver<IS> {
   public:
    using Alg = AbstractSolver<OS>; /*!< Acronym for an AbstractSolver<OS> */

    /**
     * @brief Construct a new EIG object using a fitness ratio of 0.6 and
     * novelty of 0.4 by default. Use the EIGBuilder instead of the constructor
     * for safe experimentation.
     *
     * @param fitness Ratio of fitness for the WFF
     * @param novelty Ratio of novelty for the WFF
     */
    EIG(const float &fitness = 0.6f, const float &novelty = 0.4f);

    virtual ~EIG() = default;

    void run() override;

    [[nodiscard]] json to_json() const override;

    /// @brief Get the name of the algorithm.
    /// @returns a string with the content EIG
    [[nodiscard]] string getName() const override { return "EIG"; };

    /// @brief Get the ID. Similar to getName.
    [[nodiscard]] string getID() const override { return getName(); };

    virtual Front<IS> getResults() const override;

    /// @brief Get a raw pointer to the Instance Generation Problem.
    /// @return
    const IP *getInstanceProblem() const { return instProb.get(); }

    /// @brief Set the Instance Generation Problem.
    /// @param problem
    void setInstanceProblem(unique_ptr<IP> problem);

    /// @brief Get the portfolio of algorithms used in the evolutionary process
    /// @return
    vector<Alg *> getPortfolio() const;

    /// @brief Set the portfolio of algorithms to use in the evolutionary
    /// process. The order of the algorithms in the portfolio is crucial. Notice
    /// that the first algorithm in the portfolio will be considered the target
    /// and therefore the instances will be generated according to its
    /// performance. Also, it is important to know the order of the remaining
    /// algorithms in order to analysis the results obtained since not tags are
    /// included in the results at this moment.
    /// @param configs
    void setPortfolio(vector<unique_ptr<Alg>> &configs);

    /**
     * @brief Get the mutation rate used in EIG for mutation instances during
     * the evolutionary process
     *
     * @return float
     */
    [[nodiscard]] float getMutationRate() const { return mutationRate; }

    /// @brief Set the mutation rate used in EIG for mutation instances during
    /// the evolutionary process
    /// @param mRate
    void setMutationRate(float mRate) { this->mutationRate = mRate; }

    /**
     * @brief Get the crossover rate used in EIG for applying the crossover
     * operator to instances during the evolutionary process
     *
     * @return float
     */
    [[nodiscard]] float getCrossoverRate() const { return crossoverRate; }

    /// @brief Set the crossover rate used in EIG for applying the crossover
    /// operator to instances during the evolutionary process
    /// @param cRate
    void setCrossoverRate(float cRate) { this->crossoverRate = cRate; }

    /**
     * @brief Get the number of repetitions that each algorithm in the portfolio
     * must perform over each instance at every generation of the EIG.
     *
     * @return int
     */
    [[nodiscard]] int getRepetitions() const { return repetitions; }

    /// @brief Set the number of repetitions that each algorithm in the
    /// portfolio must perform over each instance at every generation of the
    /// EIG. Due to stochastic nature of most algorithms used in EIG the number
    /// of repetitions should vary between 10 to 30. For deterministic
    /// heuristics can be set to one.
    ///  @param reps
    void setRepetitions(int reps) { this->repetitions = reps; }

    /// @brief Get the mutation operator used to mutate instances in EIG
    /// @return a raw pointer to Mutation<IS>
    Mutation<IS> *getMutation() const { return mutation.get(); }

    /// @brief Set the mutation operator. Takes ownership of the pointer.
    /// @param mut unique_ptr
    void setMutation(unique_ptr<Mutation<IS>> mut) {
        this->mutation = move(mut);
    }
    /// @brief Get the crossover operator used in EIG.
    /// @return a raw pointer to Crossover<IS>
    Crossover<IS> *getCrossover() const { return crossover.get(); }

    /// @brief Set the crossover operator. Takes ownership of the pointer.
    /// @param cx
    void setCrossover(unique_ptr<Crossover<IS>> cx) {
        this->crossover = move(cx);
    }
    /// @brief Get the selection operator.
    /// @return a raw pointer to Selection<IS>
    Selection<IS> *getSelection() const { return selection.get(); }

    /// @brief Set the selection operator. Takes ownership of the pointer.
    /// @param sel
    void setSelection(unique_ptr<Selection<IS>> sel) {
        this->selection = move(sel);
    }

    /**
     * @brief Get the evaluation approach for the instances.
     *
     * @return InstanceFitness*
     */
    [[nodiscard]] InstanceFitness *getEvaluation() const {
        return instanceFitness.get();
    }
    /// @brief Set the evaluation approach for the instances.
    /// @param eval
    void setInstanceFitness(unique_ptr<InstanceFitness> eval) {
        this->instanceFitness = move(eval);
    }

    /// @brief Get the number of generations that EIG must perform.
    /// @return
    inline int getGenerations() { return this->getMaxEvaluations(); };

    /// @brief Get the Novelty Search approach used in EIG.
    /// @return
    const NoveltySearch<IS, float> *getNoveltySearch() const {
        return noveltySearch.get();
    }
    /// @brief Set the Novelty Search approach to use.
    /// @param noveltySearch
    void setNoveltySearch(unique_ptr<NoveltySearch<IS, float>> noveltySearch) {
        this->noveltySearch = move(noveltySearch);
    }

    /// @brief Set the weights to use in the WFF.
    /// @param fW
    /// @param nW
    void setWeightedFunction(const float &fW, const float &nW) {
        this->weightedFitness = make_unique<Weighted<IS>>(fW, nW);
    }

   protected:
    void initProgress() override;

    void updateProgress() override;

    void finishProgress() override;

    bool isStoppingConditionReached() override;

    void createInitialPopulation() override;

    void evaluatePopulation(vector<IS> &individuals) override;

    [[maybe_unused]] void evaluateIndividual(IS &individual);

    void evaluationPhase(vector<IS> &individuals);

    void reproduction(IS &solution, IS &solution2);

    void replacement(vector<IS> &individuals);

    virtual void updateEvolution(vector<IS> &solutions);

   protected:
    unique_ptr<IP> instProb; /*!< Instance Generation Problem */
    vector<unique_ptr<Alg>>
        algPortfolio; /*!< Portfolio of algorithms to evaluate the instances */

    unique_ptr<Mutation<IS>> mutation;   /*!< Mutation operator */
    unique_ptr<Crossover<IS>> crossover; /*!< Crossover operator */
    unique_ptr<Selection<IS>> selection; /*!< Selection operator */

    float mutationRate;  /*!< Mutation rate */
    float crossoverRate; /*!< Crossover rate */
    int repetitions; /*!< Number of repetitions to perform by the algorithms */

    unique_ptr<NoveltySearch<IS, float>> noveltySearch; /*!< Novelty Search */
    unique_ptr<InstanceFitness> instanceFitness;  // Evaluation formulation
    unique_ptr<Weighted<IS>> weightedFitness;

    AverageFitness<OS> averageFitness; /*!< Avg fitness calculation for each
                                          instance at every generation */
    BestFitness<OS>
        bestFitness; /*!< Best fitness calculation for each algorithm after
                        solving a instance at every generation */
};

/**
 * @brief Creates a default EIG with only the evaluation as EasyInstances
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
EIG<IP, IS, OP, OS>::EIG(const float &fitness, const float &novelty)
    : AbstractSolver<IS>(),
      mutationRate(0.05),
      crossoverRate(0.8),
      repetitions(10),
      averageFitness(),
      bestFitness() {
    // Easy instances by default
    instanceFitness = make_unique<EasyInstances>();
    weightedFitness = make_unique<Weighted<IS>>(fitness, novelty);
    noveltySearch = make_unique<NSFeatures<IS>>(make_unique<Euclidean<float>>(),
                                                this->maxEvaluations, 3.0, 15);
    this->evolutionInterval = 1;
}

/**
 * @brief Interface to run the EIG for generating a set of instances for the
 * optimisation problem defined (OP)
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::run() {
    if (instProb) {
#ifdef DEBUG
        std::cout << "Running EIG" << std::endl;
#endif
        createInitialPopulation();
        this->startTime = chrono::system_clock::now();
        vector<IS> offspring(this->populationSize);
        while (!isStoppingConditionReached()) {
#ifdef DEBUG
            std::cout << "Performing Generation " << this->performedEvaluations
                      << std::endl;
#endif
            for (int i = 0; i < this->populationSize; i++) {
                IS firstChild = selection->select(this->population);
                IS secondChild = selection->select(this->population);
                reproduction(firstChild, secondChild);
                offspring[i] = firstChild;
            }
            evaluationPhase(offspring);
            replacement(offspring);
            // Updates the fitness evolution
            this->updateEvolution(this->population);
            updateProgress();
        }
        this->endTime = chrono::system_clock::now();
        finishProgress();
    } else {
        throw std::logic_error("AbstractDomain not set in EIG::run");
    }
}

/**
 * @brief Updates the evolution results of the algorithm for the given
 * checkpoint
 *
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param solutions
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::updateEvolution(vector<IS> &solutions) {
    if (this->performedEvaluations % 10 == 0) {
        this->evolution.update(this->performedEvaluations, solutions);
        this->avgEvolution.update(this->performedEvaluations, solutions);
    }
}

/**
 * @brief Evaluation phase of the EIG
 * All the stages necessary to evaluated the population of individuals
 * are performed from this method.
 *  1. First, each instance is solved by each
 *      algorithm in the portfolio (repeatdly if necessary) to construct the
 *      biasedFitness.
 *  2. Second, we run the novelty search approach to evaluate the diversity of
 *      the solutions within the population.
 *  3. The Weighted-Fitness-Function is used to calculate the final fitness for
 * each individual.
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param individuals
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::evaluationPhase(vector<IS> &individuals) {
    // Evaluates with the Instance type and the EAs (only Fitness)
    evaluatePopulation(individuals);
    // Computes the diversity of each individual
    individuals = noveltySearch->run(individuals, this->instProb.get());
    // Computes the weighted sum of both fit + div
    weightedFitness->computeFitness(individuals);
}

/**
 * @brief Returns a set of individuals as a result of the evolutionary method.
 * The individual are instances for the OP
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @return Front of IS solutions
 */
template <typename IP, typename IS, typename OP, typename OS>
Front<IS> EIG<IP, IS, OP, OS>::getResults() const {
    return noveltySearch->getResults();
}

/**
 * @brief Initialises the number of generations performed to 0
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::initProgress() {
    this->performedEvaluations = 0;
}

/**
 * @brief Updates the evolution by increasing the number of generations
 * performed
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::updateProgress() {
    this->performedEvaluations++;
}

/**
 * @brief Finishes the execution of the EIG
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::finishProgress() {
    std::chrono::duration<double> diff = this->endTime - this->startTime;
    this->elapsedTime = diff.count();
}

/**
 * @brief Checks whether the number of performed generations has reached the
 * maximum allowed
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @return True if the performedGenerations == maxGenerations, False otherwise
 */
template <typename IP, typename IS, typename OP, typename OS>
bool EIG<IP, IS, OP, OS>::isStoppingConditionReached() {
    return (this->performedEvaluations >= this->maxEvaluations);
}

/**
 * @brief Creates a initial population of Problem Instances of the OP type. Uses
 * the IP method "createSolutions".
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::createInitialPopulation() {
    this->population.reserve(this->populationSize);
    this->population = instProb->createSolutions(this->populationSize);
    evaluatePopulation(this->population);
    this->updateEvolution(this->population);
    initProgress();
}

/**
 * @brief Evaluates the population of individuals with all the algorithms in the
 * portfolio
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param individuals Population of IS to evaluate using the
 * configurations
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::evaluatePopulation(vector<IS> &individuals) {
    instProb->beforeEvaluation(individuals);
    for (IS &individual : individuals) {
        // Creates the optimisation problem for the EAs
        shared_ptr<OP> optProblem = instProb->genOptProblem(individual);

        vector<float> avgBestFitEAs;  // Avg. Best fitness from each alg.
        avgBestFitEAs.reserve(algPortfolio.size());
        vector<vector<float>> bestSolsEAs;  // Best #reps solutions/algorithm

        for (unique_ptr<Alg> &config : algPortfolio) {
            vector<float> bestF(repetitions, 0.0);  // Resets the bestF
            // Run the problem rep times
            for (int rep = 0; rep < repetitions; rep++) {
                config->setProblem(optProblem);
                config->run();
                // Get the results and compute the best founds
                Front<OS> front = config->getResults();
                OS bestSolution = bestFitness.getBestSolution(front);
                bestF[rep] = bestSolution.getFitness();
            }
            // Saves avg. of the best found in #reps
            float averageBestFitness = averageFitness.computeValue(bestF);
            avgBestFitEAs.push_back(averageBestFitness);
            // Saves all best #reps solutions for analysis
            bestSolsEAs.push_back(bestF);
        }

        // Saves the fitness founds to future analysis
        individual.setConfigFitness(bestSolsEAs);
        individual.setAvgConfigFitness(avgBestFitEAs);
        // Set the biased fitness depending on the type of instance to generate
        // (easy or hard)
        individual.setBiasedFitness(
            this->instanceFitness->compute(avgBestFitEAs));
    }
    // Problem dependent changes that must be executed after evaluate the
    // instances
    instProb->afterEvaluation(individuals);
}

/**
 * @brief Evaluates the given IS with all the algorithms in the porftolio
 *
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param individual IS individual to evaluate using all the
 * configurations
 */
template <typename IP, typename IS, typename OP, typename OS>
[[maybe_unused]] void EIG<IP, IS, OP, OS>::evaluateIndividual(IS &individual) {
    // Creates the optimisation problem for the EAs
    shared_ptr<OP> optProblem = instProb->genOptProblem(individual);
    vector<float> allFitness;
    allFitness.reserve(algPortfolio.size());
    for (unique_ptr<Alg> &config : algPortfolio) {
        Front<OS> solutions(repetitions);
        for (int rep = 0; rep < repetitions; rep++) {
            config->setProblem(optProblem);
            config->run();
            Front<OS> front = config->getResults();
            OS bestSolution = bestFitness.getBestSolution(front);
            solutions.addSolution(bestSolution);
        }
        // Set the fitness as the avg. of the best found in #repetitions times
        float averageBestFitness = averageFitness.computeValue(solutions);
        allFitness.push_back(averageBestFitness);
    }
    individual.setBiasedFitness(instanceFitness->compute(allFitness));
}

/**
 * @brief Executes the genetic operator over two instances to generate new
 * individuals
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param solution1 First child of type IS
 * @param solution2 Second child of type IS
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::reproduction(IS &solution1, IS &solution2) {
    // Cruzamos los individuos con cierta probabilidad
    if (PseudoRandom::randDouble() < this->crossoverRate) {
        this->crossover->run(solution1, solution2);
    }
    this->mutation->run(solution1, this->mutationRate, this->instProb.get());
}

/**
 * @brief Replacement method for the population. In this case, the
 * replacement is a simple generational approach, which also includes the
 * comparison of individuals with the archive and the feature set of the NS.
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param individuals New population of individuals to replace the current one
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::replacement(vector<IS> &individuals) {
    if (individuals.size() != this->population.size()) {
        std::string where = " replacement at EIG";
        throw(Mismatch(where));
    }
    const float probIncl = this->populationSize / 100.0;
    for (int i = 0; i < this->populationSize; i++) {
        if (cmpByFitness(individuals[i], this->population[i]) == FIRST_BEST) {
            this->population[i] = individuals[i];
        }
        if ((PseudoRandom::randDouble() > probIncl) &&
            (this->population[i].getBiasedFitness() > 0)) {
            // Always check feasibility
            this->noveltySearch->insertIntoArchive(this->population[i]);
        }
    }
    // Novelty Search procedures to include individuals in the archives
    this->noveltySearch->cmpFinals(this->population);
}

/**
 * @brief Updates the portfolio to solve the Instance Generation
 * Problem
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param configs
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::setPortfolio(vector<unique_ptr<Alg>> &configs) {
    algPortfolio.clear();
    algPortfolio.reserve(configs.size());
    for (unsigned int i = 0; i < configs.size(); i++) {
        algPortfolio.push_back(move(configs[i]));
    }
}

/**
 * @brief Sets the instance generation problem to toSolve
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @param problem
 */
template <typename IP, typename IS, typename OP, typename OS>
void EIG<IP, IS, OP, OS>::setInstanceProblem(unique_ptr<IP> problem) {
    this->instProb = move(problem);
}

/**
 * @brief Returns the portfolio used by the EIG
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @return
 */
template <typename IP, typename IS, typename OP, typename OS>
vector<AbstractSolver<OS> *> EIG<IP, IS, OP, OS>::getPortfolio() const {
    vector<Alg *> configurations;
    configurations.reserve(algPortfolio.size());
    for (const unique_ptr<Alg> &config : algPortfolio) {
        configurations.push_back(config.get());
    }
    return configurations;
}

/**
 * @brief Creates the JSON representation of the EIG
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @return
 */
template <typename IP, typename IS, typename OP, typename OS>
json EIG<IP, IS, OP, OS>::to_json() const {
    json data;
    Evolution copy = this->evolution;
    AvgEvolution avgCopy = this->avgEvolution;
    data["name"] = this->getName();
    data["max_generations"] = this->getMaxEvaluations();
    data["repetitions"] = this->repetitions;
    data["pop_size"] = this->populationSize;
    data["mutation"] = this->mutation->getName();
    data["crossover"] = this->crossover->getName();
    data["mutation_rate"] = this->mutationRate;
    data["crossover_rate"] = this->crossoverRate;
    data["elapsed_time"] = this->elapsedTime;
    data["evolution"] = copy.results();
    data["avg_evolution"] = avgCopy.results();
    data["novelty_search"] = this->noveltySearch->to_json();
    auto [fRatio, nRatio] = this->weightedFitness->getFAndNRatios();
    data["weighted_function"]["fitness_ratio"] = fRatio;
    data["weighted_function"]["novelty_ratio"] = nRatio;
    int i = 0;
    bool target = true;
    for (const unique_ptr<Alg> &config : this->algPortfolio) {
        data["portfolio"][i] = config->to_json();
        data["portfolio"][i]["isTarget"] = target;
        i++;
        target = false;
    }
    return data;
}

#endif