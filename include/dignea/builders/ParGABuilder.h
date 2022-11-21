//
// Created by amarrero on 22/6/21.
//

#ifndef DIGNEA_PARGABuilder_H
#define DIGNEA_PARGABuilder_H

#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>
#include <dignea/factories/CXFactory.h>
#include <dignea/factories/MutFactory.h>
#include <dignea/factories/SelFactory.h>

#include <iostream>

/**
 * @brief Parallel Genetic Algorithm Builder. This class creates a unique_ptr to
 * a Parallel Genetic Algorithm.
 *
 * @tparam S
 */
template <class S>
class ParGABuilder {
   public:
    static ParGABuilder<S> create();

    explicit ParGABuilder();

    virtual ~ParGABuilder() = default;

    operator unique_ptr<ParallelGeneticAlgorithm<S>>();

    ParGABuilder<S> &usingCores(const int &cores);

    ParGABuilder<S> &toSolve(unique_ptr<Problem<S>> problem);

    /**
     * @brief This method only returns *this and it is used to make the building
     * process more natural. Completely optional.
     *
     * @return ParGABuilder<S>&
     */
    ParGABuilder<S> &with() { return *this; };

    ParGABuilder<S> &crossover(CXType cxType);

    ParGABuilder<S> &crossover(unique_ptr<Crossover<S>>);

    ParGABuilder<S> &withCrossRate(const float &crossRate);

    ParGABuilder<S> &mutation(MutType mutation);

    ParGABuilder<S> &withMutRate(const float &mutRate);

    ParGABuilder<S> &selection(SelType selType);

    ParGABuilder<S> &populationOf(const int &popsize);

    ParGABuilder<S> &runDuring(const int &maxEvals);

   private:
    void newCompSet() { compSet++; }

   private:
    unique_ptr<ParallelGeneticAlgorithm<S>> ga;
    int compSet;
    bool problemSet;
    static const int MAX_COMPONENTS;
};

template <class S>
const int ParGABuilder<S>::MAX_COMPONENTS = 8;

template <class S>
ParGABuilder<S>::ParGABuilder() : compSet(0), problemSet(false) {
    this->ga = make_unique<ParallelGeneticAlgorithm<S>>();
}

/**
 * @brief Creates a ParGABuilder object
 *
 * @tparam S
 * @return ParGABuilder<S>
 */
template <class S>
ParGABuilder<S> ParGABuilder<S>::create() {
    return ParGABuilder<S>();
}

/**
 * @brief Defines the number of cores that the Parallel Genetic Algorithm will
 * be using
 *
 * @tparam S
 * @param cores
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::usingCores(const int &cores) {
    this->ga->setNumberOfCores(cores);
    this->newCompSet();
    return *this;
}

/**
 * @brief Builds the actual Parallel Genetic Algorithm using the specified
 * components and parameters.
 *
 * @tparam S
 * @return unique_ptr<ParallelGeneticAlgorithm<S>>
 */
template <class S>
ParGABuilder<S>::operator unique_ptr<ParallelGeneticAlgorithm<S>>() {
    if (!this->problemSet) {
        std::cerr << "Algorithm created but no problem set. "
                     "Remember to set a problem before calling run"
                  << std::endl;
    }
    if (this->compSet >= ParGABuilder<S>::MAX_COMPONENTS)
        return move(this->ga);
    else {
        throw runtime_error(
            "No all the components are set. ParGA requires: problem, "
            "mutation operator, crossover operator, selection operator, "
            "mutation rate, crossover rate, population size, "
            "max evaluation and number of cores");
    }
}

/**
 * @brief Defines the problem to solve with the Genetic Algorithm.  Receives a
 * unique_ptr and takes the ownership of it.
 *
 * @tparam S
 * @param problem
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::toSolve(unique_ptr<Problem<S>> problem) {
    this->ga->setProblem(move(problem));
    newCompSet();
    this->problemSet = true;
    return *this;
}

/**
 * @brief  Defines the crossover operator type to use with this Genetic
 * Algorithm. Variants are available at CXType.
 *
 * @tparam S
 * @param cx
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::crossover(CXType cx) {
    CXFactory<S> factory;
    auto crossover = factory.create(cx);
    this->ga->setCrossover(move(crossover));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the crossover operator type to use with this Genetic
 * Algorithm. Receives a unique_ptr and takes the ownership of it.
 *
 * @tparam S
 * @param cx
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::crossover(unique_ptr<Crossover<S>> cx) {
    this->ga->setCrossover(move(cx));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the crossover rate to apply the crossover operator.
 *
 * @tparam S
 * @param crossRate
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::withCrossRate(const float &crossRate) {
    this->ga->setCrossRate(crossRate);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the mutation operator type to use with this Genetic
 * Algorithm. Variants are available at MutType.
 *
 * @tparam S
 * @param mutation
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::mutation(MutType mutation) {
    MutFactory<S> factory;
    auto mut = factory.create(mutation);
    this->ga->setMutation(move(mut));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the mutation rate to apply in the mutation operator.
 *
 * @tparam S
 * @param mutRate
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::withMutRate(const float &mutRate) {
    this->ga->setMutationRate(mutRate);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the selection operator to use with this Genetic Algorithm.
 * Variants are available at SelType.
 *
 * @tparam S
 * @param selType
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::selection(SelType selType) {
    SelFactory<S> factory;
    auto sel = factory.create(selType);
    this->ga->setSelection(move(sel));
    newCompSet();
    return *this;
}

/**
 * @brief  Defines the number of individuals in the population of the Genetic
 * Algorithm.
 *
 * @tparam S
 * @param popsize
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::populationOf(const int &popsize) {
    this->ga->setPopulationSize(popsize);
    newCompSet();
    return *this;
}

/**
 * @brief Sets the number of evaluations to perform.
 *
 * @tparam S
 * @param maxEvals
 * @return ParGABuilder<S>&
 */
template <class S>
ParGABuilder<S> &ParGABuilder<S>::runDuring(const int &maxEvals) {
    this->ga->setMaxEvaluations(maxEvals);
    newCompSet();
    return *this;
}

#endif  // DIGNEA_ParGABuilder_H
