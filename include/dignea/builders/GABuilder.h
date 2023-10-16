//
// Created by amarrero on 21/6/21.
//

#ifndef __GA_BUILDER_H__
#define __GA_BUILDER_H__

#include <dignea/algorithms/multiobjective/NSGA2.h>
#include <dignea/algorithms/singleobjective/FIGA.h>
#include <dignea/algorithms/singleobjective/GenerationalGA.h>
#include <dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h>
#include <dignea/algorithms/singleobjective/SteadyGA.h>
#include <dignea/core/AbstractGA.h>
#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>
#include <dignea/factories/CXFactory.h>
#include <dignea/factories/MutFactory.h>
#include <dignea/factories/SelFactory.h>

#include <iostream>

using namespace std;

/**
 * @brief Types of GAs currently supported.
 * - Generational GA
 * - First Improve GA
 * - Parallel GA
 * - Steady State GA
 * - Non-Dominated Sorting Genetic Algorithm II
 *
 */
enum class GAType { Generational, FIGA, Parallel, Steady, NSGA2 };

/**
 * @brief Genetic Algorithm Factory. This function allows to create default
 * Genetic Algorithm configuration on the fly.
 *
 * @tparam S
 * @param type GAType of the Genetic Algorithm to create
 * @return unique_ptr<AbstractGA<S>>
 */
template <class S>
unique_ptr<AbstractGA<S>> createDefaultGA(GAType type) {
    map<GAType, function<unique_ptr<AbstractGA<S>>()>> factories;
    factories[GAType::FIGA] = [] { return make_unique<FIGA<S>>(); };
    factories[GAType::Generational] = [] {
        return make_unique<GenerationalGA<S>>();
    };
    factories[GAType::Steady] = [] { return make_unique<SteadyGA<S>>(); };
    factories[GAType::Parallel] = [] {
        return make_unique<ParallelGeneticAlgorithm<S>>();
    };
    factories[GAType::NSGA2] = [] { return make_unique<NSGA2<S>>(); };

    return factories[type]();
}

/**
 * @brief Genetic Algorithm Builder class. This class allows to create Genetic
 * Algorithm configurations using a builder pattern.
 *
 * @tparam S
 */
template <class S>
class GABuilder {
   public:
    static GABuilder<S> create(GAType type);

    explicit GABuilder(GAType type);

    ~GABuilder() = default;

    operator unique_ptr<AbstractGA<S>>();

    GABuilder<S> &toSolve(unique_ptr<Problem<S>> problem);

    /**
     * @brief This method only returns *this and it is used to make the building
     * process more natural. Completely optional.
     *
     * @return GABuilder<S>&
     */
    GABuilder<S> &with() { return *this; };

    GABuilder<S> &crossover(CXType cxType);

    GABuilder<S> &crossover(unique_ptr<Crossover<S>>);

    GABuilder<S> &withCrossRate(const float &crossRate);

    GABuilder<S> &mutation(MutType mutation);

    GABuilder<S> &withMutRate(const float &mutRate);

    GABuilder<S> &selection(SelType selType);

    GABuilder<S> &populationOf(const int &popsize);

    GABuilder<S> &runDuring(const int &maxEvals);

   private:
    void newCompSet() { compSet++; }

   protected:
    GABuilder() = default;

   private:
    static const int MUST_COMPS;

   protected:
    unique_ptr<AbstractGA<S>> ga;
    GAType type;
    int compSet;
    bool problemSet;
};

template <class S>
const int GABuilder<S>::MUST_COMPS = 7;

/**
 * @brief Creates a GABuilder object to generate a "type" Genetic Algorithm
 *
 * @tparam S
 * @param type
 * @return GABuilder<S>
 */
template <class S>
GABuilder<S> GABuilder<S>::create(GAType type) {
    return GABuilder<S>{type};
}

/**
 * @brief Construct a new GABuilder<S>::GABuilder
 *
 * @tparam S
 * @param type Genetic Algorithm variant to create
 */
template <class S>
GABuilder<S>::GABuilder(GAType type)
    : type(type), compSet(0), problemSet(false) {
    if (type == GAType::Parallel) {
        throw runtime_error(
            "To create a Parallel GA use the ParGABuilder class");
    }
    ga = createDefaultGA<S>(type);
}

/**
 * @brief Defines the problem to solve with the Genetic Algorithm.  Receives a
 * unique_ptr and takes the ownership of it.
 *
 * @tparam S
 * @param problem
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::toSolve(unique_ptr<Problem<S>> problem) {
    this->ga->setProblem(move(problem));
    newCompSet();
    this->problemSet = true;
    return *this;
}

/**
 * @brief Defines the crossover operator type to use with this Genetic
 * Algorithm. Variants are available at CXType.
 *
 * @tparam S
 * @param cx
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::crossover(CXType cx) {
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
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::crossover(unique_ptr<Crossover<S>> cx) {
    this->ga->setCrossover(move(cx));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the crossover rate to apply the crossover operator.
 *
 * @tparam S
 * @param crossRate
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::withCrossRate(const float &crossRate) {
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
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::mutation(MutType mutation) {
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
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::withMutRate(const float &mutRate) {
    this->ga->setMutationRate(mutRate);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the selection operator to use with this Genetic Algorithm.
 * Variants are available at SelType.
 * @tparam S
 * @param selType
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::selection(SelType selType) {
    SelFactory<S> factory;
    auto sel = factory.create(selType);
    this->ga->setSelection(move(sel));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the number of individuals in the population of the Genetic
 * Algorithm.
 *
 * @tparam S
 * @param popsize
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::populationOf(const int &popsize) {
    this->ga->setPopulationSize(popsize);
    newCompSet();
    return *this;
}

/**
 * @brief Sets the number of evaluations to perform.
 *
 * @tparam S
 * @param maxEvals
 * @return GABuilder<S>&
 */
template <class S>
GABuilder<S> &GABuilder<S>::runDuring(const int &maxEvals) {
    this->ga->setMaxEvaluations(maxEvals);
    newCompSet();
    return *this;
}

/**
 * @brief Builds the Genetic Algorithm with all the components specified and
 * returns a unique_ptr
 *
 * @tparam S
 * @return unique_ptr<AbstractGA<S>>
 */
template <class S>
GABuilder<S>::operator unique_ptr<AbstractGA<S>>() {
    if (!this->problemSet) {
        std::cerr << "Algorithm created but no problem set. "
                     "Remember to set a problem before calling run"
                  << std::endl;
    }
    if (compSet >= MUST_COMPS)
        return move(this->ga);
    else {
        throw runtime_error(
            "No all the components are set. GA requires: problem, "
            "mutation operator, crossover operator, selection operator, "
            "mutation rate, crossover rate, population size "
            "and max evaluations");
    }
}

#endif  //__GA_BUILDER_H__
