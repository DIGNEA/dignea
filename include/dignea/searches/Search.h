//
// Created by amarrero on 18/6/21.
//

#ifndef DIGNEA_SEARCH_H
#define DIGNEA_SEARCH_H

#include <dignea/core/Front.h>
#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>

#include <vector>

using namespace std;

/**
 * @brief Search algorithm interface for dignea. Use this class as the skeleton
 * to define local or other types of searches.
 *
 * @tparam S
 */
template <typename S>
class Search {
   public:
    /// @brief Constructs a default Search
    Search() = default;

    virtual ~Search() = default;
    /// @brief Perform a search over a population.
    /// @param  population Population of individuals to take as the starting
    /// point
    /// @param  prob Raw pointer to the problem being solved
    /// @return New population of solutions
    virtual vector<S> run(vector<S> &population, const Problem<S> *prob) = 0;
    /// @brief Returns a Front of solutions. Use this method for searches that
    /// must keep track of solutions or run several times
    /// @return Front<S> of solutions found
    virtual Front<S> getResults() = 0;
};

#endif  // DIGNEA_SEARCH_H
