
/**
 * @file MapElites.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-03-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef DIGNEA_MAP_ELITES_H
#define DIGNEA_MAP_ELITES_H

#include <dignea/generator/EIG.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <map>
#include <numeric>
#include <ranges>
#include <tuple>
#include <utility>
#include <vector>

#include "NumCpp.hpp"
#include "NumCpp/Functions/unique.hpp"
#include "NumCpp/NdArray.hpp"
using namespace std;
/**
 * @brief FeatureInfo defines the information for a specific feature.
 *  - Lower Limit of the values
 *  - Upper Limit of the values
 *  - Number of bins to create
 *
 */
using FeatureInfo = std::tuple<float, float, int>;

/**
 * @brief  Features defines the information a feature.
 *  - ID of the feature.
 *  - FeatureInfoType
 *
 */
using Features = std::map<int, FeatureInfo>;

using Key = std::array<float, 8>;

using Bins = nc::NdArray<float>;

/**
 * @brief Instance Generation Algorithm. Known as Meta-Evolutionary Algorithm
 * (MapElites). This algorithm uses a Weighted Fitness Function (WFF) to
 * evaluate how biased and diverse are the instances generated during the
 * evolutionary process. The instances generated should be biased to the
 * performance of the target algorithm in the portfolio but also should show
 * some diversity between them.
 *
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
class MapElites : public EIG<IP, IS, OP, OS> {
   public:
    using Alg = AbstractEA<OS>; /*!< Acronym for an AbstractEA<OS> */
    using Value = IS;
    using Grid = std::map<Key, Value>;

    MapElites();

    virtual ~MapElites() = default;

    void run() override;

    [[nodiscard]] json to_json() const override;

    /// @brief Get the name of the algorithm.
    /// @returns a string with the content MapElites
    [[nodiscard]] string getName() const override { return "MapElites"; };

    virtual Front<IS> getResults() const override;

    void setFeaturesInfo(const Features &f);

   protected:
    void createInitialPopulation() override;

    void evaluatePopulation(vector<IS> &individuals) override;

   private:
    void mapToGrid(vector<IS> &individuals);

    Key constructKey(const vector<float> &features);

    vector<IS> gridToVector();

   private:
    Features features;  // Feature information for the domain
    Grid grid;          // Grid where the best instances are represented
    array<Bins, 8> bins;
};

/**
 * @brief Creates a default MapElites with only the evaluation as EasyInstances
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
MapElites<IP, IS, OP, OS>::MapElites()
    : EIG<IP, IS, OP, OS>(), features(), grid(), bins() {
    this->weightedFitness = nullptr;
    this->noveltySearch = nullptr;
}

/**
 * @brief Converts the grid into a vector shape with only the instances
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam OS
 * @return vector<IS>
 */
template <typename IP, typename IS, typename OP, typename OS>
vector<IS> MapElites<IP, IS, OP, OS>::gridToVector() {
    vector<IS> v;
    std::transform(this->grid.begin(), this->grid.end(), std::back_inserter(v),
                   [](auto &entry) { return entry.second; });
    return v;
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
void MapElites<IP, IS, OP, OS>::createInitialPopulation() {
    this->population.reserve(this->populationSize);
    this->population = this->instProb->createSolutions(this->populationSize);
    this->evaluatePopulation(this->population);
    this->mapToGrid(this->population);
    this->updateEvolution(this->population);
    this->initProgress();
}

/**
 * @brief Interface to run the MapElites for generating a set of instances for
 * the optimisation problem defined (OP)
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 */
template <typename IP, typename IS, typename OP, typename OS>
void MapElites<IP, IS, OP, OS>::run() {
    if (this->features.empty()) {
        throw std::logic_error("Features not set in MapElites::run");
    }
    if (this->instProb) {
#ifdef DEBUG
        std::cout << "Running MapElites" << std::endl;
#endif
        this->createInitialPopulation();
        this->startTime = chrono::system_clock::now();
        while (!this->isStoppingConditionReached()) {
#ifdef DEBUG
            std::cout << "Performing Generation " << this->performedEvaluations
                      << std::endl;
#endif
            vector<IS> offspring(this->populationSize);
            vector pop = this->gridToVector();
            if (pop.empty()) {
                pop = this->population;
            }

            for (int i = 0; i < this->populationSize; i++) {
                IS child = this->selection->select(pop);
                // this->reproduction(firstChild, secondChild);
                //  Vanilla MapElites only perform mutation
                this->mutation->run(child, this->mutationRate,
                                    this->instProb.get());
                offspring[i] = child;
            }

            this->evaluatePopulation(offspring);
            this->mapToGrid(offspring);

            // Updates the fitness evolution
            this->updateEvolution(this->population);
            this->updateProgress();
        }
        this->endTime = chrono::system_clock::now();
        this->finishProgress();

    } else {
        throw std::logic_error("Problem not set in MapElites::run");
    }
    // Creates the population so we avoid the `this' argument discards
    // qualifiers in getResults method
    this->population.clear();
    this->population = gridToVector();
}

/**
 * @brief Creates the pre-defined bins for each feature in the domain
 * Creates the linspace with nBins different value for each feature
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam OS
 * @param individuals
 */
template <typename IP, typename IS, typename OP, typename OS>
void MapElites<IP, IS, OP, OS>::setFeaturesInfo(const Features &f) {
    this->features = f;
    // Calculate the bins
    for (int i = 0; i < 8; i++) {
        auto [start, stop, num] = this->features[i];
        bins[i] = nc::linspace(start, stop, num, true);
    }
}

/**
 * @brief Creates a Key for an instance descriptor. The Key contains the bin in
 * the feature space for each ith-feature in the descriptor.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam OS
 * @param descriptor
 * @return Key
 */
template <typename IP, typename IS, typename OP, typename OS>
Key MapElites<IP, IS, OP, OS>::constructKey(const vector<float> &descriptor) {
    Key key;
    // Traducimos a los bins
    for (unsigned int i = 0; i < descriptor.size(); i++) {
        //  Creates a vector with a single value
        const nc::NdArray<float> x = {descriptor[i]};
        const nc::NdArray<float> bin = this->bins[i];

        const auto uniqueBins = unique(bin);
        auto result = nc::NdArray<float>(1, x.size());
        result.fill(0);

        typename decltype(result)::size_type idx{0};
        std::for_each(x.begin(), x.end(),
                      [&uniqueBins, &result, &idx](const auto value) {
                          const auto upperBin = std::upper_bound(
                              uniqueBins.begin(), uniqueBins.end(), value);
                          result[idx++] = static_cast<float>(
                              std::distance(uniqueBins.begin(), upperBin));
                      });

        key[i] = result[0];
    }
    return key;
}

/**
 * @brief Calculate the mapping between the feature values and the corresponding
 * bin in the bins vector
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam OS
 * @param individuals
 */
template <typename IP, typename IS, typename OP, typename OS>
void MapElites<IP, IS, OP, OS>::mapToGrid(vector<IS> &individuals) {
    // Para cada individuo, obtenemos su descriptor
    for (IS &ind : individuals) {
        // Only consider individuals with positive fitness
        if (ind.getFitness() > 0) {
            vector descriptor = ind.getFeatures();
            Key key = this->constructKey(descriptor);
            // Tries to insert the individual in the grid
            // If there is not any solution in the bin we include ind
            if (!this->grid.contains(key)) {
                this->grid.insert({key, ind});
                // If there's a solution but ind has better fitness, update
            } else if (ind.getFitness() > this->grid[key].getFitness()) {
                this->grid[key] = ind;
            }
        }
    }
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
void MapElites<IP, IS, OP, OS>::evaluatePopulation(vector<IS> &individuals) {
    this->instProb->beforeEvaluation(individuals);
    for (IS &individual : individuals) {
        // Creates the optimisation problem for the EAs
        shared_ptr<OP> optProblem = this->instProb->genOptProblem(individual);

        vector<float> avgBestFitEAs;  // Avg. Best fitness from each alg.
        avgBestFitEAs.reserve(this->algPortfolio.size());
        vector<vector<float>> bestSolsEAs;  // Best #reps solutions/algorithm

        for (unique_ptr<Alg> &config : this->algPortfolio) {
            vector<float> bestF(this->repetitions, 0.0);  // Resets the bestF
            // Run the problem rep times
            for (int rep = 0; rep < this->repetitions; rep++) {
                config->setProblem(optProblem);
                config->run();
                // Get the results and compute the best founds
                Front<OS> front = config->getResults();
                OS bestSolution = this->bestFitness.getBestSolution(front);
                bestF[rep] = bestSolution.getFitness();
            }
            // Saves avg. of the best found in #reps
            float averageBestFitness = this->averageFitness.computeValue(bestF);
            avgBestFitEAs.push_back(averageBestFitness);
            // Saves all best #reps solutions for analysis
            bestSolsEAs.push_back(bestF);
        }

        // Saves the fitness founds to future analysis
        individual.setConfigFitness(bestSolsEAs);
        individual.setAvgConfigFitness(avgBestFitEAs);
        // Set the biased fitness depending on the type of instance to generate
        // (easy or hard)
        auto f = this->instanceFitness->compute(avgBestFitEAs);
        individual.setBiasedFitness(f);
        individual.setFitness(f);
    }
    // Problem dependent changes that must be executed after evaluate the
    // instances
    this->instProb->afterEvaluation(individuals);
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
Front<IS> MapElites<IP, IS, OP, OS>::getResults() const {
    return Front<IS>(this->population);
}

/**
 * @brief Creates the JSON representation of the MapElites
 * @tparam IP Type of Instance Problem to toSolve
 * @tparam IS Type of instances of the Optimisation Problem
 * @tparam OP Optimisation problem to toSolve with the configurations
 * @tparam OS Solutions to the OP type
 * @return
 */
template <typename IP, typename IS, typename OP, typename OS>
json MapElites<IP, IS, OP, OS>::to_json() const {
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
    int i = 0;
    bool target = true;
    for (const unique_ptr<Alg> &config : this->algPortfolio) {
        data["portfolio"][i] = config->to_json();
        data["portfolio"][i]["isTarget"] = target;
        i++;
        target = false;
    }
    data["features_info"] = this->features;
    return data;
}

#endif