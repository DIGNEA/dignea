//
// Created by amarrero on 23/6/21.
//

#ifndef DIGNEA_MEABUILDER_H
#define DIGNEA_MEABUILDER_H

#include <dignea/factories/CXFactory.h>
#include <dignea/factories/MutFactory.h>
#include <dignea/factories/NSFactory.h>
#include <dignea/factories/SelFactory.h>
#include <dignea/mea/MEA.h>
#include <dignea/mea/MEAProblem.h>
#include <dignea/searches/NSFeatures.h>
#include <dignea/searches/NSPerformance.h>

/**
 * @brief Class which represents a Builder for the MEA.
 * Creates a MEA step by step from all the different components.
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 */
template <typename IP, typename IS, typename OP, typename S>
class MEABuilder {
    using EA = unique_ptr<AbstractEA<S>>;

   public:
    static MEABuilder<IP, IS, OP, S> create();

    virtual ~MEABuilder() = default;

    operator unique_ptr<MEA<IP, IS, OP, S>>();

    MEABuilder<IP, IS, OP, S> &toSolve(unique_ptr<IP> problem);

    MEABuilder<IP, IS, OP, S> &weights(const float &fW, const float &mW);

    /**
     * @brief This method only returns *this and it is used to make the building
     * process more natural. Completely optional.
     *
     * @return MEABuilder<IP, IS, OP, S>&
     */
    MEABuilder<IP, IS, OP, S> &with() { return *this; };

    MEABuilder<IP, IS, OP, S> &portfolio(vector<EA> &configs);

    MEABuilder<IP, IS, OP, S> &withSearch(NSType nsType,
                                          unique_ptr<Distance<float>> dist,
                                          const float &thres, const int k);

    MEABuilder<IP, IS, OP, S> &evalWith(unique_ptr<InstanceFitness> evalMethod);

    MEABuilder<IP, IS, OP, S> &repeating(const int &reps);

    MEABuilder<IP, IS, OP, S> &crossover(CXType cxType);

    MEABuilder<IP, IS, OP, S> &withCrossRate(const float &crSsRate);

    MEABuilder<IP, IS, OP, S> &mutation(MutType mutation);

    MEABuilder<IP, IS, OP, S> &withMutRate(const float &mutRate);

    MEABuilder<IP, IS, OP, S> &selection(SelType selType);

    MEABuilder<IP, IS, OP, S> &populationOf(const int &popsize);

    MEABuilder<IP, IS, OP, S> &runDuring(const int &generations);

   private:
    MEABuilder();

    void newCompSet() { compSet++; }

   private:
    unique_ptr<MEA<IP, IS, OP, S>> mea;
    int compSet;
    bool problemSet;
    static const int MUST_COMPS;
};

template <typename IP, typename IS, typename OP, typename S>
const int MEABuilder<IP, IS, OP, S>::MUST_COMPS = 12;

/**
 * @brief Creates a MEABuilder object
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @return MEABuilder<IP, IS, OP, S>
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> MEABuilder<IP, IS, OP, S>::create() {
    return MEABuilder<IP, IS, OP, S>();
}

/**
 * @brief Construct a new MEABuilder<IP, IS, OP, S>::MEABuilder. Use the
 * create() method instead of this.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S>::MEABuilder() : compSet(0), problemSet(false) {
    this->mea = make_unique<MEA<IP, IS, OP, S>>();
}

/**
 * @brief Builds a MEA object with all the components specified and returns a
 * unique_ptr.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @return unique_ptr<MEA<IP, IS, OP, S>>
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S>::operator unique_ptr<MEA<IP, IS, OP, S>>() {
    if (!this->problemSet) {
        std::cerr << "MEA created but not problem set. "
                     "Remember to set a problem before calling run"
                  << std::endl;
    }
    if (compSet >= MUST_COMPS) {
        return move(this->mea);
    } else {
        throw runtime_error(
            "No all the components are set. MEA requires: "
            "configurations, evaluation method,"
            "Novelty Search configuration, "
            "mutation operator, crSsover operator, "
            "selection operator, mutation rate, "
            "crSsover rate, population size, "
            "max generations and repetitions");
    }
}

/**
 * @brief Defines the MEAProblem to solve.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param problem
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::toSolve(
    unique_ptr<IP> problem) {
    this->mea->setInstanceProblem(move(problem));
    newCompSet();
    this->problemSet = true;
    return *this;
}

/**
 * @brief Sets the weights that will be used in the Weighted Fitness Function.
 * Notice that fW + nW = 1.0.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param fW
 * @param nW
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::weights(const float &fW,
                                                              const float &nW) {
    this->mea->setWeightedFunction(fW, nW);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the portfolio of solvers to evaluate the instances during the
 * evolutionary process. The first algorithm in the vector will be considered as
 * the target for the instances.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param configs
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::portfolio(
    vector<EA> &configs) {
    this->mea->setPortfolio(configs);
    newCompSet();
    return *this;
}

/**
 * @brief Novelty search configuration for the MEA. Here we define the Novelty
 * Approach (Features, Performance), the distance Metric, the threshold to
 * include a solution in the NS archive and the 'k' value to use in the KNN
 * method during the NS execution.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param nsType
 * @param dist
 * @param thres
 * @param k
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::withSearch(
    NSType nsType, unique_ptr<Distance<float>> dist, const float &thres,
    const int k) {
    NSFactory<IS, float> factory;
    auto ns = factory.create(nsType, move(dist), thres, k);
    this->mea->setNoveltySearch(move(ns));
    newCompSet();
    return *this;
}

/**
 * @brief Evaluation approach for generating instances. The instances must be
 * considered easy to solve for the target or hard.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param evalMethod
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::evalWith(
    unique_ptr<InstanceFitness> evalMethod) {
    this->mea->setInstanceFitness(move(evalMethod));
    newCompSet();
    return *this;
}

/**
 * @brief Defines how many repetitions the algorithms in the portfolio must
 * perform over each instance in each generation of the MEA
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param reps
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::repeating(
    const int &reps) {
    this->mea->setRepetitions(reps);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the crossover operator type to use with this Genetic
 * Algorithm. Variants are available at CXType
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param cxType
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::crossover(CXType cxType) {
    CXFactory<IS> factory;
    auto cx = factory.create(cxType);
    this->mea->setCrossover(move(cx));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the crossover rate to apply the crossover operator.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param crSsRate
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::withCrossRate(
    const float &crSsRate) {
    this->mea->setCrossoverRate(crSsRate);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the mutation operator type to use with this Genetic
 * Algorithm. Variants are available at MutType.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param mutation
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::mutation(
    MutType mutation) {
    MutFactory<IS> factory;
    auto mut = factory.create(mutation);
    this->mea->setMutation(move(mut));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the mutation rate to apply in the mutation operator.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param mutRate
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::withMutRate(
    const float &mutRate) {
    this->mea->setMutationRate(mutRate);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the selection operator to use. Variants are available at
 * SelType.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param selType
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::selection(
    SelType selType) {
    SelFactory<IS> factory;
    auto sel = factory.create(selType);
    this->mea->setSelection(move(sel));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the population of instances to use. This is the number of
 * instances that the MEA will be evolving at each generation.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param popsize
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::populationOf(
    const int &popsize) {
    this->mea->setPopulationSize(popsize);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the number of generations to perform by MEA. Notice that the
 * MEA works by generations instead of evaluations as others algorithms in
 * dignea do.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param generations
 * @return MEABuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
MEABuilder<IP, IS, OP, S> &MEABuilder<IP, IS, OP, S>::runDuring(
    const int &generations) {
    this->mea->setMaxEvaluations(generations);
    newCompSet();
    return *this;
}

#endif  // DIGNEA_MEABUILDER_H
