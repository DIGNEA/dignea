/**
 * @file Experiment.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2020-12-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_EXPERIMENT_H
#define DIGNEA_EXPERIMENT_H

#include <dignea/core/AbstractEA.h>
#include <dignea/core/Front.h>
#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>
#include <dignea/metrics/BestFitness.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using json = nlohmann::json;

/**
 * @brief Class to define computational experiments to perform
 * The concept is to define and algorithm and a problem instance to be solved
 * several times.
 * @tparam S
 */
template <class S>
class Experiment {
   public:
    Experiment(shared_ptr<Problem<S>> problem,
               unique_ptr<AbstractEA<S>> algorithm, const std::string &expName,
               const int &reps);

    virtual ~Experiment() = default;

    /// @brief Get a raw pointer to the problem to solve.
    /// @return Pointer to problem
    Problem<S> *getProblemToSolve() const { return problemToSolve.get(); }

    /// @brief Set the problem to solve. Uses a shared_ptr.
    /// @param problem
    void setProblemToSolve(shared_ptr<Problem<S>> problem) {
        this->problemToSolve = problem;
    }

    /// @brief Get a raw pointer to the algorithm used in the experiment
    /// @return Pointer to algorithm
    AbstractEA<S> *getAlgorithm() const { return algorithm.get(); }

    /// @brief Set the algorithm to solve the problem. Uses a unique_ptr and
    /// takes ownership of the pointer.
    /// @param algo
    void setAlgorithm(unique_ptr<AbstractEA<S>> algo) {
        this->algorithm = move(algo);
    }

    /// @brief Get the number of repetitions to perform
    /// @return Repetitions (int)
    int getRepetitions() const { return repetitions; }

    /// @brief Set the number of repetitions to perform
    /// @param reps
    void setRepetitions(int reps) { this->repetitions = reps; }

    /// @brief Get the experiment name
    /// @return Experiment name as string
    const string &getName() const { return name; }

    /// @brief Set the name of the experiment
    /// @param newName
    void setName(const string &newName) { this->name = newName; }

    bool run();

    /// @brief Get the results of each repetition
    /// @return Vector of Front<S> with the results of each repetition
    vector<Front<S>> getResults() const { return this->repsSolutions; }

    /// @brief Get the best fitnesses found after performing the experiment.
    /// @return Vector of floats with best fitness.
    vector<float> getBestFounds() const { return this->bestFounds; }

    json to_json();

   private:
    shared_ptr<Problem<S>>
        problemToSolve; /*!< Problem to solve in the experiment */
    unique_ptr<AbstractEA<S>>
        algorithm;                  /*!< Algorithm used in the experiment */
    string name;                    /*!< Name of the experiment*/
    int repetitions;                /*!< Repetitions to perform */
    vector<Front<S>> repsSolutions; /*!< All results */
    vector<float> bestFounds;       /*!< Best results found */
};

/**
 * @brief Construct a new Experiment.
 *
 * @tparam S
 * @param problem shared_ptr to a Problem to solve
 * @param algorithm unique_ptr to an EA to solve the problem
 * @param expName Name of the experiment for the json representation
 * @param reps Number of times that the algorithm must solve the problem
 */
template <class S>
Experiment<S>::Experiment(shared_ptr<Problem<S>> problem,
                          unique_ptr<AbstractEA<S>> algorithm,
                          const std::string &expName, const int &reps)
    : name(expName), repetitions(0) {
    this->problemToSolve = problem;
    this->algorithm = move(algorithm);
    this->repetitions = reps;
    // Sets the problem to the algorithm
    this->algorithm->setProblem(this->problemToSolve);
    this->repsSolutions.reserve(reps);
    this->bestFounds.resize(reps, 0.0f);
}

/**
 * @brief Performs the experiment. It solves the problem several times and
 * collects the best solution found in each repetition.
 *
 * @tparam S
 * @return true if everything runs correctly
 * @return false otherwise
 */
template <class S>
bool Experiment<S>::run() {
    std::cout << "Running experiment: " << std::endl;
    std::cout << "\t- Problem: " << problemToSolve->getName() << std::endl;
    std::cout << "\t- Algorithm: " << algorithm->getName() << std::endl;
    std::cout << "\t- Repetitions: " << repetitions << std::endl;
    std::cout << "\t- Max. Evaluations: " << algorithm->getMaxEvaluations()
              << std::endl;

    BestFitness<S> bestFitness;
    int rep = 0;
    while (rep < repetitions) {
        algorithm->setProblem(problemToSolve);
        algorithm->run();
        if (!algorithm->getResults().empty()) {
            Front<S> solutions = algorithm->getResults();
            repsSolutions.push_back(solutions);
            bestFounds[rep] = bestFitness.computeValue(solutions);
            rep++;
        }
    }
    return true;
}

/**
 * @brief Creates and returns a JSON object with all the information of the
 * experiment. It contains the name of the experiment, the JSON information of
 * the problem, the JSON information of the algorithm, the best solution found
 * for each repetition and the Front<S> of each repetition containing all
 * results.
 *
 * @tparam S
 * @return json
 */
template <class S>
json Experiment<S>::to_json() {
    json data;
    data["name"] = name;
    data["problem"] = problemToSolve->to_json();
    data["algorithm"] = algorithm->to_json();
    for (int i = 0; i < repetitions; i++) {
        data["reps_results"][i] = this->repsSolutions[i].to_json();
    }
    data["bests"] = this->bestFounds;
    return data;
}

#endif  // DIGNEA_EXPERIMENT_H
