/**
 * @file BPPInstance.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DIGNEA_BPPINSTANCE_H
#define DIGNEA_BPPINSTANCE_H

#include <dignea/generator/AbstractInstance.h>

/**
 * @brief  Instance Bin-Packing Problem Solution.
 * Class which represents the solution for the Instance Bin-Packing Problem.
 * This class is used to represent an actual BPP instance.
 *
 * -    The variables inherited from the Solution<int, int> class contains the
 * weights of the items [w0,...,wn-1] in a single array.
 *
 * -    The capacity is stored separately.
 *
 */
class BPPInstance : public AbstractInstance<int, int> {
   public:
    BPPInstance();

    virtual ~BPPInstance() = default;

    explicit BPPInstance(const int &nVars, const int &nObjs = 1);

    BPPInstance(const BPPInstance &solution);

    explicit BPPInstance(const BPPInstance *solution);

    BPPInstance(unique_ptr<BPPInstance> &ptr) = delete;

    BPPInstance &operator=(const BPPInstance &copy);

    bool operator==(const BPPInstance &other) const;

    /// @brief Get the features.
    vector<float> getFeatures() const override;

    void setVariables(const vector<int> &vars) override;

    json to_json() const override;

    virtual void to_instance(std::ostream &os) const;

    inline int getCapacity() const { return this->capacity; }

    inline void setCapacity(int q) { this->capacity = q; }

   protected:
    int capacity;
};

#endif  // DIGNEA_BPPINSTANCE_H
