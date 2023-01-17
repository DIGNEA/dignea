/**
 * @file KPInstance.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 8/3/21
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_EKPSOLUTION_H
#define DIGNEA_EKPSOLUTION_H

#include <dignea/generator/AbstractInstance.h>

/**
 * @brief  Instance Knapsack Problem Solution.
 *      Class which represents the solution for the Instance Knapsack
 * Problem (EKPP). This class is used to represent an actual Knapsack Problem
 * instance.
 * -    The variables inherited from the Solution<int, float> class contains the
 * pairs
 *      [(w0,p0), ..., (wn-1,pn-1)] in a single array.
 * -    The capacity is stored separately.
 *
 */
class KPInstance : public AbstractInstance<int, float> {
   public:
    KPInstance();

    explicit KPInstance(const int &nVars);

    KPInstance(const KPInstance &solution);

    explicit KPInstance(const KPInstance *solution);

    KPInstance(unique_ptr<KPInstance> &ptr) = delete;

    virtual ~KPInstance() = default;

    inline void setCapacity(const int &Q) { this->capacity = Q; };

    inline int getCapacity(void) const { return this->capacity; };

    void setVariables(const vector<int> &vars) override;

    json to_json() const override;

    virtual void to_instance(std::ostream &os) const;

   protected:
    int capacity;  // Capacity of the Knapsack
};

#endif  // DIGNEA_EKPSOLUTION_H
