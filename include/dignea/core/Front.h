//
// Created by amarrero on 24/3/21.
//

#ifndef DIGNEA_FRONT_H
#define DIGNEA_FRONT_H

#include <dignea/core/Problem.h>
#include <dignea/utilities/Comparator.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
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

    void addSolution(const S &solution, const Problem<S> *problem);

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
Front<S>::Front(const vector<S> &sols)
    : numOfSolutions(sols.size()), solutions{} {
    copy(sols.begin(), sols.end(), back_inserter(this->solutions));
}

/**
 * @brief Construct a new Front object copying an existing one
 *
 * @tparam S
 * @param copy Front to copy.
 */
template <class S>
Front<S>::Front(const Front<S> &copy)
    : numOfSolutions(copy.numOfSolutions), solutions(copy.solutions) {}

/**
 * @brief Construct a new Front object copying an existing one.
 *
 * @tparam S
 * @param copy Pointer to the front to copy.
 */
template <class S>
Front<S>::Front(const Front<S> *copy)
    : numOfSolutions(copy->numOfSolutions), solutions(copy->solutions) {}

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
 * This is only for single-objective solutions
 * @tparam S
 * @param solution
 */
template <class S>
void Front<S>::addSolution(const S &solution) {
    if (solution.getNumberOfObjs() != 1) {
        string where =
            "Using addSolution for single-objective solutions. Please "
            "consider using addSolution(const S&, const Problem<S>*) instead";
        throw runtime_error(where);
    }
    solutions.push_back(solution);
    numOfSolutions++;
}

/**
 * @brief Includes a new solution in the front.
 *
 * @tparam S
 * @param solution
 */
template <class S>
void Front<S>::addSolution(const S &solution, const Problem<S> *problem) {
    if (solution.getNumberOfObjs() == 1) {
        this->addSolution(solution);
        std::cerr << "Using addSolution for multi-objective solutions. Please "
                     "consider using addSolution(const S& solution) instead"
                  << std::endl;
    } else {
        unsigned int idx = 0;
        while (idx < solutions.size()) {
            auto dominates = dominanceTest(solution, solutions[idx], problem);
            if ((dominates == SECOND_DOMINATES) ||
                (dominates == NON_DOMINANCES_EQUALS)) {
                return;
            } else if (dominates == FIRST_DOMINATES) {
                // Removes all dominated solutions
                solutions[idx] = solutions[solutions.size() - 1];
                solutions.pop_back();
                this->numOfSolutions--;
            }
            idx++;
        }
        solutions.push_back(solution);
        numOfSolutions++;
    }
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
    int i = 0;
    for (auto solution : this->solutions) {
        data[to_string(i)] = solution.to_json();
        i++;
    }
    return data;
}

#endif  // DIGNEA_FRONT_H
