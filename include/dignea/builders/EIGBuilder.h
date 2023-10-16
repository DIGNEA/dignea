//
// Created by amarrero on 23/6/21.
//

#ifndef DIGNEA_EIGBUILDER_H
#define DIGNEA_EIGBUILDER_H

#include <dignea/factories/CXFactory.h>
#include <dignea/factories/MutFactory.h>
#include <dignea/factories/NSFactory.h>
#include <dignea/factories/RepFactory.h>
#include <dignea/factories/SelFactory.h>
#include <dignea/generator/AbstractDomain.h>
#include <dignea/generator/EIG.h>
// #include <dignea/generator/MOEIG.h>
#include <dignea/generator/MapElites.h>
#include <dignea/searches/NSFeatures.h>
#include <dignea/searches/NSPerformance.h>

/**
 * @brief
 *
 */
enum class GeneratorType { LinearScaled, MultiObjective, MapElites };

/**
 * @brief Class which represents a Builder for the EIG.
 * Creates a EIG step by step from all the different components.
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 */
template <typename IP, typename IS, typename OP, typename S>
class EIGBuilder {
    using EA = unique_ptr<AbstractEA<S>>;

   public:
    static EIGBuilder<IP, IS, OP, S> create(GeneratorType type);

    virtual ~EIGBuilder() = default;

    operator unique_ptr<EIG<IP, IS, OP, S>>();

    EIGBuilder<IP, IS, OP, S> &toSolve(unique_ptr<IP> problem);

    EIGBuilder<IP, IS, OP, S> &weights(const float &fW, const float &mW);

    /**
     * @brief This method only returns *this and it is used to make the building
     * process more natural. Completely optional.
     *
     * @return EIGBuilder<IP, IS, OP, S>&
     */
    EIGBuilder<IP, IS, OP, S> &with() { return *this; };

    EIGBuilder<IP, IS, OP, S> &portfolio(vector<EA> &configs);

    EIGBuilder<IP, IS, OP, S> &withSearch(NSType nsType,
                                          unique_ptr<Distance<float>> dist,
                                          const float &thres,
                                          const float &finalThresh, const int k,
                                          bool warmUp = false);

    EIGBuilder<IP, IS, OP, S> &evalWith(unique_ptr<InstanceFitness> evalMethod);

    EIGBuilder<IP, IS, OP, S> &repeating(const int &reps);

    EIGBuilder<IP, IS, OP, S> &crossover(CXType cxType);

    EIGBuilder<IP, IS, OP, S> &withCrossRate(const float &crSsRate);

    EIGBuilder<IP, IS, OP, S> &mutation(MutType mutation);

    EIGBuilder<IP, IS, OP, S> &withMutRate(const float &mutRate);

    EIGBuilder<IP, IS, OP, S> &selection(SelType selType);

    EIGBuilder<IP, IS, OP, S> &replacement(ReplacementTypes reType);

    EIGBuilder<IP, IS, OP, S> &populationOf(const int &popsize);

    EIGBuilder<IP, IS, OP, S> &runDuring(const int &generations);

    EIGBuilder<IP, IS, OP, S> &setFeatures(const Features &f);

   private:
    EIGBuilder(GeneratorType t);

    void newCompSet() { compSet++; }

   private:
    unique_ptr<EIG<IP, IS, OP, S>> generator;
    int compSet;
    bool problemSet;
    GeneratorType type;
    static int MUST_COMPS;
};

template <typename IP, typename IS, typename OP, typename S>
int EIGBuilder<IP, IS, OP, S>::MUST_COMPS = 12;

/**
 * @brief Creates a EIGBuilder object
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @return EIGBuilder<IP, IS, OP, S>
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> EIGBuilder<IP, IS, OP, S>::create(GeneratorType t) {
    return EIGBuilder<IP, IS, OP, S>(t);
}

/**
 * @brief Construct a new EIGBuilder<IP, IS, OP, S>::EIGBuilder. Use the
 * create() method instead of this.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S>::EIGBuilder(GeneratorType t)
    : compSet(0), problemSet(false), type(t) {
    this->type = t;  // Saves the type we're building
    switch (type) {
        case GeneratorType::LinearScaled:
            this->generator = make_unique<EIG<IP, IS, OP, S>>();
            break;
        // case GeneratorType::MultiObjective:
        //     this->generator = make_unique<MOEIG<IP, IS, OP, S>>();
        //     break;
        case GeneratorType::MapElites:
            MUST_COMPS = 10;  // MapElites does not need NS parameters
            this->generator = make_unique<MapElites<IP, IS, OP, S>>();
            break;
        default:
            std::cerr << "Unknown type in EIGBuilder. LinearScaled algorithm "
                         "created by default"
                      << std::endl;
            this->generator = make_unique<EIG<IP, IS, OP, S>>();

            break;
    }
}

/**
 * @brief Builds a EIG object with all the components specified and returns a
 * unique_ptr.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @return unique_ptr<EIG<IP, IS, OP, S>>
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S>::operator unique_ptr<EIG<IP, IS, OP, S>>() {
    if (!this->problemSet) {
        std::cerr << "EIG created but not problem set. "
                     "Remember to set a problem before calling run"
                  << std::endl;
    }
    if (compSet >= MUST_COMPS) {
        return move(this->generator);
    } else {
        throw runtime_error(
            "No all the components are set. EIG requires: "
            "configurations, evaluation method,"
            "Novelty Search configuration, "
            "mutation operator, crSsover operator, "
            "selection operator, mutation rate, "
            "crSsover rate, population size, "
            "max generations and repetitions");
    }
}

/**
 * @brief Defines the AbstractDomain to solve.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param problem
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::toSolve(
    unique_ptr<IP> problem) {
    this->generator->setInstanceProblem(move(problem));
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::weights(const float &fW,
                                                              const float &nW) {
    this->generator->setWeightedFunction(fW, nW);
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::portfolio(
    vector<EA> &configs) {
    this->generator->setPortfolio(configs);
    newCompSet();
    return *this;
}

/**
 * @brief Novelty search configuration for the EIG. Here we define the Novelty
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::withSearch(
    NSType nsType, unique_ptr<Distance<float>> dist, const float &thres,
    const float &finalThresh, const int k, bool warmUp) {
    NSFactory<IS> factory;
    auto ns = factory.create(nsType, move(dist), thres, finalThresh, k, warmUp);
    this->generator->setNoveltySearch(move(ns));
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::evalWith(
    unique_ptr<InstanceFitness> evalMethod) {
    this->generator->setInstanceFitness(move(evalMethod));
    newCompSet();
    return *this;
}

/**
 * @brief Defines how many repetitions the algorithms in the portfolio must
 * perform over each instance in each generation of the EIG
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param reps
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::repeating(
    const int &reps) {
    this->generator->setRepetitions(reps);
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::crossover(CXType cxType) {
    CXFactory<IS> factory;
    auto cx = factory.create(cxType);
    this->generator->setCrossover(move(cx));
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::withCrossRate(
    const float &crSsRate) {
    this->generator->setCrossoverRate(crSsRate);
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::mutation(
    MutType mutation) {
    MutFactory<IS> factory;
    auto mut = factory.create(mutation);
    this->generator->setMutation(move(mut));
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::withMutRate(
    const float &mutRate) {
    this->generator->setMutationRate(mutRate);
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
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::selection(
    SelType selType) {
    SelFactory<IS> factory;
    auto sel = factory.create(selType);
    this->generator->setSelection(move(sel));
    newCompSet();
    return *this;
}

template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::replacement(
    ReplacementTypes repType) {
    RepFactory<IS> factory;
    auto rep = factory.create(repType);
    this->generator->setReplacement(move(rep));
    newCompSet();
    return *this;
}

/**
 * @brief Defines the population of instances to use. This is the number of
 * instances that the EIG will be evolving at each generation.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param popsize
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::populationOf(
    const int &popsize) {
    this->generator->setPopulationSize(popsize);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the number of generations to perform by EIG. Notice that the
 * EIG works by generations instead of evaluations as others algorithms in
 * dignea do.
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param generations
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::runDuring(
    const int &generations) {
    this->generator->setMaxEvaluations(generations);
    newCompSet();
    return *this;
}

/**
 * @brief Defines the Feature information for MapElites
 *
 * @tparam IP
 * @tparam IS
 * @tparam OP
 * @tparam S
 * @param generations
 * @return EIGBuilder<IP, IS, OP, S>&
 */
template <typename IP, typename IS, typename OP, typename S>
EIGBuilder<IP, IS, OP, S> &EIGBuilder<IP, IS, OP, S>::setFeatures(
    const Features &f) {
    if (this->type == GeneratorType::MapElites) {
        static_cast<MapElites<IP, IS, OP, S> *>(this->generator.get())
            ->setFeaturesInfo(f);
        newCompSet();
        return *this;
    }
    return *this;
}

#endif  // DIGNEA_EIGBUILDER_H
