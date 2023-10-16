

#ifndef DIGNEA_HEURISTIC_H
#define DIGNEA_HEURISTIC_H

#include <dignea/core/AbstractEA.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Base heuristic class. This is the skeleton for future heuristic
 * implementations. Extends AbstractEA for compatibility with EIG.
 *
 * @tparam S Type of the solutions
 */
template <class S>
class Heuristic : public AbstractEA<S> {
   public:
    Heuristic() = default;

    virtual ~Heuristic() = default;

    virtual void run() = 0;

    virtual string getName() const = 0;

    virtual string getID() const = 0;

    virtual json to_json() const;

    virtual Front<S> getResults() const = 0;

   protected:
    /**
     * @brief Not implemented. Nothing really to do here.
     *
     */
    virtual void initProgress(){};

    /**
     * @brief Not implemented. Nothing really to do here.
     *
     */
    virtual void updateProgress(){};

    void finishProgress() override;

    /**
     * @brief Always returns false. TODO: Refactor to inherit from Algorithm.
     *
     * @return false
     */
    bool isStoppingConditionReached() override { return false; };

    /**
     * @brief Not implemented. Nothing really to do here.
     *
     */
    virtual void createInitialPopulation(){};

    /**
     * @brief Not implemented. Nothing really to do here.
     *
     */
    virtual void evaluatePopulation(vector<S> &){};

    /**
     * @brief Not implemented. Nothing really to do here.
     *
     */
    virtual void updateEvolution(vector<S> &){};

    /**
     * @brief Not implemented. Nothing really to do here.
     *
     */
    virtual void updateEvolution(const int &checkpoint, vector<S> &){};
};

/**
 * @brief Finishes the progress of the Heuristic by computing the elapsed time.
 *
 * @tparam S
 */
template <class S>
void Heuristic<S>::finishProgress() {
    std::chrono::duration<double> diff = this->endTime - this->startTime;
    this->elapsedTime = diff.count();
}
/**
 * @brief Returns the JSON representation of the Heuristic
 *
 * @tparam S
 * @return json
 */
template <class S>
json Heuristic<S>::to_json() const {
    json data;
    data["name"] = this->getName();
    return data;
}
#endif  // DIGNEA_HEURISTIC_H