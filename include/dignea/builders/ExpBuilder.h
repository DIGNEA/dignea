//
// Created by amarrero on 30/12/20.
//

#ifndef DIGNEA_EXPBUILDER_H
#define DIGNEA_EXPBUILDER_H

#include <dignea/experiment/Experiment.h>

#include <filesystem>
#include <string>
#include <utility>

/**
 * @brief Experiment Builder which allows to create a experiment
 * @tparam Problem
 * @tparam S
 */
template <class S>
class ExpBuilder {
    using EA = unique_ptr<AbstractEA<S>>;

   public:
    static ExpBuilder<S> create(const string &experimentName);

    virtual ~ExpBuilder() = default;

    ExpBuilder<S> &saveResultsIn(const std::string &);

    ExpBuilder<S> &toSolve(unique_ptr<Problem<S>> problem);

    ExpBuilder<S> &usingAlgorithm(EA algorithm);

    ExpBuilder<S> &repeat(const int &reps);

    ExpBuilder<S> &withRunner(bool inParallel = false);

    operator unique_ptr<Experiment<S>>();

   private:
    explicit ExpBuilder(string name);

   private:
    string outputPath;
    string experimentName;
    int repetitions;
    EA evolutionaryAlgorithm;
    unique_ptr<Problem<S>> problem;
    bool includeRunner;
    bool parallelMode;
};

/**
 * @brief Creates an Experiment Builder setting the experiment name to the given
 * value in the string.
 *
 * @tparam S
 * @param experimentName
 * @return ExpBuilder<S>
 */
template <class S>
ExpBuilder<S> ExpBuilder<S>::create(const string &experimentName) {
    return ExpBuilder<S>{experimentName};
}

/**
 * Creates a ExpBuilder with default values. The string `name` will be used to
 * set the name of the experiment
 * @tparam Problem
 * @tparam S
 */
template <class S>
ExpBuilder<S>::ExpBuilder(std::string name)
    : outputPath(), experimentName(std::move(name)), repetitions(0) {
    this->evolutionaryAlgorithm = nullptr;
    this->problem = nullptr;
}

/**
 * Sets the output path to the results of the experiment.
 * Also creates the directory if it does not exists.
 * @tparam Problem
 * @tparam S
 * @return
 */
template <class S>
ExpBuilder<S> &ExpBuilder<S>::saveResultsIn(const string &path) {
    if (!std::filesystem::exists(path)) {
        std::cout << "The directory: " << path
                  << " does not exists. Creating..." << std::endl;
        if (!std::filesystem::create_directories(path)) {
            throw(runtime_error(
                "Cannot created directories for the results in ExpBuilder"));
        }
    }
    this->outputPath = path;
    return *this;
}

/**
 * Sets the problem to to solve in the experiment. Receives a unique_ptr and
 * takes the ownership of it.
 * @tparam Problem
 * @tparam S
 * @param problem
 * @return
 */
template <class S>
ExpBuilder<S> &ExpBuilder<S>::toSolve(unique_ptr<Problem<S>> prob) {
    this->problem = move(prob);
    return *this;
}

/**
 * Sets the algorithm which solves the optimisation problem. Receives a
 * unique_ptr and takes the ownership of it.
 * @tparam Problem
 * @tparam S
 * @param algorithm
 * @return
 */
template <class S>
ExpBuilder<S> &ExpBuilder<S>::usingAlgorithm(
    unique_ptr<AbstractEA<S>> algorithm) {
    this->evolutionaryAlgorithm = move(algorithm);
    return *this;
}

/**
 * Sets the repetitions to perform
 * @tparam Problem
 * @tparam S
 * @param reps
 * @return
 */
template <class S>
ExpBuilder<S> &ExpBuilder<S>::repeat(const int &reps) {
    this->repetitions = reps;
    return *this;
}

/**
 * @brief Specifies the parallel approach to solve the problem.
 *
 * @tparam S
 * @param inParallel
 * @return ExpBuilder<S>&
 */
template <class S>
ExpBuilder<S> &ExpBuilder<S>::withRunner(bool inParallel) {
    this->includeRunner = true;
    this->parallelMode = inParallel;
    return *this;
}

/**
 * @brief Operator to create a unique_ptr to an Experiment object with all the
 * configuration previously assigned
 *
 * @tparam S
 * @return unique_ptr<Experiment<S>>
 */
template <class S>
ExpBuilder<S>::operator unique_ptr<Experiment<S>>() {
    if (!this->evolutionaryAlgorithm) {
        throw std::runtime_error(
            "EA not set in ExpBuilder. Use usingAlgorithm()");
    }
    if (!this->problem) {
        throw std::runtime_error(
            "Problem not set in ExpBuilder. Use toSolve()");
    }
    if (outputPath.empty()) {
        throw std::runtime_error(
            "Output path not set in ExpBuilder. Use saveResultsIn()");
    }

    if (repetitions == 0) {
        throw std::runtime_error(
            "Repetitions equal to 0 in ExpBuilder. Use repeat()");
    }
    string finalName = this->outputPath + "/" + this->experimentName;
    auto experiment = make_unique<Experiment<S>>(
        move(problem), move(evolutionaryAlgorithm), finalName, repetitions);
    return experiment;
}

#endif  // DIGNEA_EXPBUILDER_H
