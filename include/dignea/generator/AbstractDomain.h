/**
 * @file AbstractDomain.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 8/3/21.
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_EIGPROBLEM_H
#define DIGNEA_EIGPROBLEM_H

#include <dignea/core/Problem.h>

/**
 * @brief EIG Instance Generation Problem.
 *  Abstract Problem for the EIG.
 *  This problem will be used as a superclass for any new instance generation
 *  problem. This is the type of problem for the EIG, not for the evaluation
 *  phase. It acts as a wrapper for the EIG.
 *
 * @tparam OptProblem
 * @tparam InstSolution
 */
template <typename OptProblem, typename InstSolution>
class AbstractDomain : public Problem<InstSolution> {
   public:
    AbstractDomain();

    AbstractDomain(const int &numberOfVars, const int &numberOfObjectives,
               const int &nCons);

    virtual ~AbstractDomain() = default;

    /**
     * @brief Generates a shared_ptr to an optimisation problem represented in
     * the InstSolution given as a parameter
     *
     * @param instance Reference to a InstSolution type
     * @return shared_ptr<OptProblem>
     */
    virtual shared_ptr<OptProblem> genOptProblem(
        const InstSolution &instance) const = 0;

    /**
     * @brief Generates and returns a vector of InstSolutions. Instances of the
     * optimisation problem.
     *
     * @param maxSolutions population size
     * @return vector<InstSolution>
     */
    virtual vector<InstSolution> createSolutions(
        const int &maxSolutions) const = 0;

    /**
     * @brief Method to apply operations to the instances before evaluate them
     * This is problem dependent
     *
     */
    virtual void beforeEvaluation(vector<InstSolution> &) = 0;
    /**
     * @brief  Method to apply operations to the instances after evaluate them
     *  This is problem dependent
     */
    virtual void afterEvaluation(vector<InstSolution> &) = 0;

   protected:
    int internalDimension;
};

/**
 * @brief Construct a new AbstractDomain<OptProblem, InstSolution>::AbstractDomain
 * object
 *
 * @tparam OptSolution Solution type of the optimisation problem.
 * @tparam InstSolution Solution type of the EIG problem.
 */
template <typename OptProblem, typename InstSolution>
AbstractDomain<OptProblem, InstSolution>::AbstractDomain() : Problem<InstSolution>() {}

/**
 * @brief Construct a new AbstractDomain<OptProblem, InstSolution>::AbstractDomain
 * object
 *
 * @tparam OptSolution Solution type of the optimisation problem.
 * @tparam InstSolution Solution type of the EIG problem.
 * @param numberOfVars Integer representing the dimnesion (number of variables)
 * of the solution
 * @param numberOfObjectives Integer representing the number of objectives of
 * the solution
 * @param nCons Integer representing the number of constraints of the solution
 */
template <typename OptProblem, typename InstSolution>
AbstractDomain<OptProblem, InstSolution>::AbstractDomain(const int &numberOfVars,
                                                 const int &numberOfObjectives,
                                                 const int &nCons)
    : Problem<InstSolution>(numberOfVars, numberOfObjectives, nCons) {}

#endif  // DIGNEA_EIGPROBLEM_H
