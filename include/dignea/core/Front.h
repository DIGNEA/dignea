//
// Created by amarrero on 24/3/21.
//

#ifndef DIGNEA_FRONT_H
#define DIGNEA_FRONT_H

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

/**
 * @brief Front class which stores the final results of an EA execution.
 *
 * @tparam S Type of individual in the population
 */
template <class S>
class Front {
   public:
    Front(const int &nSolutions);

    Front(const vector<S> &solutions);

    Front(const Front<S> &copy);

    Front(const Front<S> *copy);

    virtual ~Front();

    void setSolutions(const vector<S> &solutions);

    vector<S> getSolutions() const;

    void addSolution(const S &solution);

    /**
     * @brief Gets the number of solutions in the front
     *
     * @return int
     */
    int getNumOfSolutions() const { return numOfSolutions; }
    /**
     * @brief Checks whether the front is empty or not
     *
     * @return true
     * @return false
     */
    bool empty() const { return solutions.empty(); }

    json to_json() const;

   private:
    int numOfSolutions;  /*!< Number of solutions in the front */
    vector<S> solutions; /*!< Vector with the solutions in the front */
};

/**
 * @brief Construct a new Front object
 *
 * @tparam S
 * @param nSolutions number of solutions to set in the front
 */
template <class S>
Front<S>::Front(const int &nSolutions) : numOfSolutions(nSolutions) {}

/**
 * @brief Construct a new Front object with the solutions in the vector sols.
 *
 * @tparam S
 * @param sols Solutions to insert in the front.
 */
template <class S>
Front<S>::Front(const vector<S> &sols) {
    numOfSolutions = sols.size();
    copy(sols.begin(), sols.end(), back_inserter(this->solutions));
}

/**
 * @brief Construct a new Front object copying an existing one
 *
 * @tparam S
 * @param copy Front to copy.
 */
template <class S>
Front<S>::Front(const Front<S> &copy) {
    numOfSolutions = copy.numOfSolutions;
    solutions = copy.solutions;
}

/**
 * @brief Construct a new Front object copying an existing one.
 *
 * @tparam S
 * @param copy Pointer to the front to copy.
 */
template <class S>
Front<S>::Front(const Front<S> *copy) {
    this->numOfSolutions = copy->numOfSolutions;
    this->solutions = copy->solutions;
}

template <class S>
Front<S>::~Front() {
    solutions.clear();
}

/**
 * @brief Resets the front with the solutions in the vector<S> solutions.
 *
 * @tparam S
 * @param solutions
 */
template <class S>
void Front<S>::setSolutions(const vector<S> &solutions) {
    this->solutions.clear();
    this->solutions = solutions;
    numOfSolutions = solutions.size();
}

/**
 * @brief Returns the solutions in the front.
 *
 * @tparam S
 * @return vector<S>
 */
template <class S>
vector<S> Front<S>::getSolutions() const {
    return this->solutions;
}

/**
 * @brief Includes a new solution in the front.
 *
 * @tparam S
 * @param solution
 */
template <class S>
void Front<S>::addSolution(const S &solution) {
    solutions.push_back(solution);
    numOfSolutions++;
}

/**
 * @brief Creates a JSON object with the information of the Front
 * @tparam S
 * @return json object with the front information
 */
template <class S>
json Front<S>::to_json() const {
    json data;
    data["n_solutions"] = numOfSolutions;
    for (int i = 0; i < numOfSolutions; i++) {
        data[to_string(i)] = solutions[i].to_json();
    }
    return data;
}

#endif  // DIGNEA_FRONT_H
