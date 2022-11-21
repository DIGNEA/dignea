//
// Created by amarrero on 22/4/21.
//

#ifndef DIGNEA_REPLACEMENT_H
#define DIGNEA_REPLACEMENT_H

#include <dignea/core/Solution.h>

/**
 * @brief Replacement skeleton operator.
 *
 * @tparam S Type of individual in the population
 */
template <class S>
class Replacement {
   public:
    Replacement() = default;

    virtual ~Replacement() = default;

   public:
    /**
     *  @brief Method to perform the replacement operation. This is a virtual
     * method that must be implemented in the subclasses. It receives two
     * vectors of individuals and performs the corresponding replacement over
     * it.
     *
     * @return vector<S>
     */
    virtual vector<S> replace(vector<S> &, vector<S> &) = 0;

    /**
     *  @brief Method to perform the replacement operation. This is a virtual
     * method that must be implemented in the subclasses. It receives a
     * population of individuals, a new individual to be included and a position
     * and performs the corresponding replacement over the population.
     *
     * @param position
     * @return vector<S>
     */
    virtual vector<S> replace(vector<S> &, const S &, const int &position) = 0;

    virtual std::string getName() const = 0;
};

#endif  // DIGNEA_REPLACEMENT_H
