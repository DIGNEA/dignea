//
// Created by amarrero on 2/12/20.
//

#ifndef DIGNEA_SOLUTION_H
#define DIGNEA_SOLUTION_H

#include <dignea/utilities/exceptions/OutOfRange.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

/**
 * @brief Class to represent a solution to the optimization problems in the tool
 *
 * @tparam V Type of the variables of the solution (float, bool, int, etc)
 * @tparam O Type of the objectives of the solution (float, bool, int, etc)
 */
template <typename V, typename O>
class Solution {
   public:
    Solution();

    virtual ~Solution();

    Solution(const int &nVars, const int &nObjs);

    Solution(const int &nVars, const int &nObjs, const int &nCons);

    Solution(const Solution<V, O> &);

    Solution(const Solution<V, O> *);

    Solution &operator=(const Solution &copy);

    bool operator==(const Solution &other) const;

    virtual unique_ptr<Solution<V, O>> clone();

    /**
     * @brief Returns the objective values of the solution
     *
     * @return const vector<O>&
     */
    virtual const vector<O> &getObjectives() const { return objectives; }

    virtual void setObjectives(const vector<O> &objs);

    /**
     * @brief Get the variables of the solution
     *
     * @return const vector<V>&
     */
    virtual const vector<V> &getVariables() const { return variables; }

    virtual void setVariables(const vector<V> &vars);

    /**
     * @brief Get the constraint values of the solution
     *
     * @return const vector<O>&
     */
    virtual const vector<O> &getConstraints() const { return constraints; }

    virtual void setConstraints(const vector<O> &constr);
    /**
     * @brief Get the number of variables (dimension) of the solution
     *
     * @return int
     */
    virtual int getNumberOfVars() const { return nVars; }

    /**
     * @brief Set the number of variables (dimension) of the solution
     *
     * @param numberOfVars
     */
    virtual void setNumberOfVars(const int &numberOfVars) {
        this->nVars = numberOfVars;
    }

    /**
     * @brief Get the number of objectives of the solution
     *
     * @return int
     */
    virtual int getNumberOfObjs() const { return nObjs; }

    /**
     * @brief Set the number of objectives of the solution
     *
     * @param numberOfObjs
     */
    virtual void setNumberOfbjs(const int &numberOfObjs) {
        this->nObjs = numberOfObjs;
    }

    /**
     * @brief Get the number of constraint in the solution
     *
     * @return int
     */
    virtual int getNumberOfCons() const { return nCons; }

    /**
     * @brief Set the number of constraints of the solution
     *
     * @param nConstraints
     */
    virtual void setNumberOfCons(const int &nConstraints) {
        this->nCons = nConstraints;
    }

    virtual void setVarAt(const int &, const V &v);

    virtual void setObjAt(const int &, const O &obj);

    virtual void setConstAt(const int &, const O &cons);

    virtual V getVarAt(const int &index) const;

    virtual O getObjAt(const int &index) const;

    virtual O getConstAt(const int &index) const;

    /**
     * @brief Get the fitness of the solution.
     *
     * @return float
     */
    virtual float getFitness() const { return fitness; };

    /**
     * @brief Set the fitness of the solution
     *
     * @param f
     */
    virtual void setFitness(const float &f) { fitness = f; };

    /**
     * @brief Get the constraint coefficient of the solutions. This value means
     * different for every problem. In most cases is the weighted sum of
     * constraint violation values.
     *
     * @return float
     */
    float getConstraintCoeff() const { return constraintCoeff; }

    /**
     * @brief Set the constraint coefficient value
     *
     * @param constraintCoeff
     */
    void setConstraintCoeff(float constraintCoeff) {
        Solution::constraintCoeff = constraintCoeff;
    }

    virtual json to_json() const;

   protected:
    int nVars;     /*!<  Number of variables of the solution (dimension). */
    int nObjs;     /*!<  Number of objectives of the solution. */
    int nCons;     /*!<  Number of constraints of the solution. */
    float fitness; /*!<  Fitness value of the solution. Always float. */
    float constraintCoeff; /*!<  Coefficient of constraint violation. */

    vector<O> objectives;  /*!<  Objective values of the solution */
    vector<V> variables;   /*!<  Variable values of the solution (genotype) */
    vector<O> constraints; /*!<  Constraint values of the solution */
};

/**
 * @brief Creates a Solution with all parameters set to zero.
 *
 * @tparam V
 * @tparam O
 */
template <typename V, typename O>
Solution<V, O>::Solution()
    : nVars(0),
      nObjs(0),
      nCons(0),
      fitness(0.0f),
      constraintCoeff(0.0f),
      objectives({}),
      variables({}),
      constraints({}) {}

/**
 * @brief Creates a new solution with the given dimension (nVars) and nObjs
 * number of objectives
 *
 * @tparam V
 * @tparam O
 * @param nVars
 * @param nObjs
 */
template <typename V, typename O>
Solution<V, O>::Solution(const int &nVars, const int &nObjs)
    : nVars(nVars),
      nObjs(nObjs),
      nCons(0),
      fitness(0.0f),
      constraintCoeff(0.0f),
      constraints({}) {
    variables.resize(nVars, (V)0);
    objectives.resize(nObjs, (O)0.0);
}

/**
 * @brief Creates a new solution with the given dimension (nVars), nObjs
 * number of objectives and nCons number of constraints
 *
 * @tparam V
 * @tparam O
 * @param nVars
 * @param nObjs
 * @param nCons
 */
template <typename V, typename O>
Solution<V, O>::Solution(const int &nVars, const int &nObjs, const int &nCons)
    : nVars(nVars),
      nObjs(nObjs),
      nCons(nCons),
      fitness(0.0f),
      constraintCoeff(0.0f) {
    variables.resize(nVars, (V)0);
    objectives.resize(nObjs, (O)0);
    constraints.resize(nCons, (O)0);
}

/**
 * @brief Creates a new solution with the information from copy.
 *
 * @tparam V
 * @tparam O
 * @param copy
 */
template <typename V, typename O>
Solution<V, O>::Solution(const Solution<V, O> &copy) {
    nVars = copy.nVars;
    nObjs = copy.nObjs;
    nCons = copy.nCons;
    fitness = copy.fitness;
    constraintCoeff = copy.constraintCoeff;
    variables.resize(nVars);
    objectives.resize(nObjs);
    constraints.resize(nCons);
    for (int i = 0; i < nVars; i++) {
        variables[i] = copy.variables[i];
    }
    for (int i = 0; i < nObjs; i++) {
        objectives[i] = copy.objectives[i];
    }
    for (int i = 0; i < nCons; i++) {
        constraints[i] = copy.constraints[i];
    }
}

/**
 * @brief Creates a new solution with the information from the pointer to other
 * solution
 *
 * @tparam V
 * @tparam O
 * @param copy
 */
template <typename V, typename O>
Solution<V, O>::Solution(const Solution<V, O> *copy) {
    if (copy == nullptr) {
        throw(runtime_error(
            "Copy Solution is nullptr in Solution(Solution* copy)"));
    }
    this->nVars = copy->nVars;
    this->nObjs = copy->nObjs;
    this->nCons = copy->nCons;
    this->fitness = copy->fitness;
    this->constraintCoeff = copy->constraintCoeff;
    this->variables.resize(this->nVars);
    this->objectives.resize(this->nObjs);
    this->constraints.resize(this->nCons);
    for (int i = 0; i < this->nVars; i++) {
        this->variables[i] = copy->variables[i];
    }
    for (int i = 0; i < this->nObjs; i++) {
        this->objectives[i] = copy->objectives[i];
    }
    for (int i = 0; i < this->nCons; i++) {
        this->constraints[i] = copy->constraints[i];
    }
}

template <typename V, typename O>
Solution<V, O>::~Solution() {
    objectives.clear();
    variables.clear();
    constraints.clear();
    objectives.shrink_to_fit();
    variables.shrink_to_fit();
    constraints.shrink_to_fit();
}

/**\deprecated Clones the solution and returns a new unique_ptr with the exact
 * same information.
 *
 *
 * @tparam V
 * @tparam O
 * @return unique_ptr<Solution<V, O>>
 */
template <typename V, typename O>
unique_ptr<Solution<V, O>> Solution<V, O>::clone() {
    auto solution = make_unique<Solution<V, O>>(nVars, nObjs, nCons);
    solution->fitness = this->fitness;
    solution->constraintCoeff = this->constraintCoeff;
    for (int i = 0; i < this->nVars; i++) {
        solution->variables[i] = this->variables[i];
    }
    for (int i = 0; i < this->nObjs; i++) {
        solution->objectives[i] = this->objectives[i];
    }
    for (int i = 0; i < this->nCons; i++) {
        solution->constraints[i] = this->constraints[i];
    }
    return solution;
}

/**
 * @brief Updates the ith variable of the solution with the value v
 *
 * @tparam V
 * @tparam O
 * @param index
 * @param v
 */
template <typename V, typename O>
void Solution<V, O>::setVarAt(const int &index, const V &v) {
    if (index < 0 || index >= this->nVars) {
        std::string where = "setVarAt with index = " + to_string(index);
        throw(OutOfRange(where));
    }
    this->variables[index] = v;
}

/**
 * @brief Updates the ith objective with the value obj
 *
 * @tparam V
 * @tparam O
 * @param index
 * @param obj
 */
template <typename V, typename O>
void Solution<V, O>::setObjAt(const int &index, const O &obj) {
    if (index < 0 || index >= this->nObjs) {
        std::string where = "setObjAt with index = " + to_string(index);
        throw(OutOfRange(where));
    }
    this->objectives[index] = obj;
}

/**
 * @brief Returns the ith variable if its in the valid range
 *
 * @tparam V
 * @tparam O
 * @param index
 * @return V
 */
template <typename V, typename O>
V Solution<V, O>::getVarAt(const int &index) const {
    if (index < 0 || index >= this->nVars) {
        std::string where = "getVarAt with index = " + to_string(index);
        throw(OutOfRange(where));
    }
    return this->variables[index];
}

/**
 * @brief Returns the ith objective value if exists
 *
 * @tparam V
 * @tparam O
 * @param index
 * @return O
 */
template <typename V, typename O>
O Solution<V, O>::getObjAt(const int &index) const {
    if (index < 0 || index >= this->nObjs) {
        std::string where = "getObjAt with index = " + to_string(index);
        throw(OutOfRange(where));
    }
    return this->objectives[index];
}

/**
 * @brief Updates the ith constraint value with the value
 *
 * @tparam V
 * @tparam O
 * @param index
 * @param value
 */
template <typename V, typename O>
void Solution<V, O>::setConstAt(const int &index, const O &value) {
    if (index < 0 || index >= this->nCons) {
        std::string where = "setConstAt with index = " + to_string(index);
        throw(OutOfRange(where));
    }
    this->constraints[index] = value;
}

/**
 * @brief Returns the ith constraint value if exists
 *
 * @tparam V
 * @tparam O
 * @param index
 * @return O
 */
template <typename V, typename O>
O Solution<V, O>::getConstAt(const int &index) const {
    if (index < 0 || index >= this->nCons) {
        std::string where = "getConstAt with index = " + to_string(index);
        throw(OutOfRange(where));
    }
    return this->constraints[index];
}

/**
 * @brief Updates the objective values of the solution.
 *
 * @tparam V
 * @tparam O
 * @param objs
 */
template <typename V, typename O>
void Solution<V, O>::setObjectives(const vector<O> &objs) {
    if (objs.size() != (unsigned int)this->nObjs) {
        std::string where = "objs.size() != nObjs in Solution::setObjectives";
        throw(OutOfRange(where));
    }
    this->objectives = objs;
}

/**
 * @brief Updates the variable values (dimension) of the solution.
 *
 * @tparam V
 * @tparam O
 * @param vars
 */
template <typename V, typename O>
void Solution<V, O>::setVariables(const vector<V> &vars) {
    if (vars.size() != (unsigned int)this->nVars) {
        auto vsize = vars.size();
        std::string where = "vars.size() = " + to_string(vsize) +
                            " != nVars = " + to_string(this->nVars) +
                            " in Solution::setVariables";
        throw(OutOfRange(where));
    }
    this->variables = vars;
}

/**
 * @brief Updates the constraints of the solution
 *
 * @tparam V
 * @tparam O
 * @param constr
 */
template <typename V, typename O>
void Solution<V, O>::setConstraints(const vector<O> &constr) {
    if (constr.size() != (unsigned int)this->nCons) {
        std::string where =
            "constr.size() != nConst in Solution::setConstraints";
        throw(OutOfRange(where));
    }
    this->constraints = constr;
}

/**
 * @brief Copies a solution using the assignment operator
 *
 * @tparam V
 * @tparam O
 * @param copy
 * @return Solution<V, O>&
 */
template <typename V, typename O>
Solution<V, O> &Solution<V, O>::operator=(const Solution &copy) {
    if (this == &copy) {
        return *this;
    }
    nVars = copy.nVars;
    nObjs = copy.nObjs;
    nCons = copy.nCons;
    fitness = copy.fitness;
    constraintCoeff = copy.constraintCoeff;
    variables = copy.variables;
    objectives = copy.objectives;
    constraints = copy.constraints;

    return *this;
}

/**
 * @brief Compares whether two solutions are equal
 *
 * @tparam V
 * @tparam O
 * @param other
 * @return true
 * @return false
 */
template <typename V, typename O>
bool Solution<V, O>::operator==(const Solution &other) const {
    if (other.fitness != this->fitness) {
        return false;
    }
    if ((this->nVars != other.nVars) || (this->nObjs != other.nObjs) ||
        (this->nCons != other.nCons)) {
        return false;
    }
    for (int i = 0; i < other.nVars; i++) {
        if (other.variables[i] != this->variables[i]) {
            return false;
        }
    }
    for (int i = 0; i < other.nObjs; i++) {
        if (other.objectives[i] != this->objectives[i]) {
            return false;
        }
    }
    for (int i = 0; i < other.nCons; i++) {
        if (other.constraints[i] != this->constraints[i]) {
            return false;
        }
    }
    if (other.constraintCoeff != this->constraintCoeff) {
        return false;
    }
    return true;
}

/**
 * @brief Creates and returns a JSON representation of the solution
 * @tparam V
 * @tparam O
 * @return a JSON object
 */
template <typename V, typename O>
json Solution<V, O>::to_json() const {
    json data;
    data["num_vars"] = this->nVars;
    data["num_objs"] = this->nObjs;
    data["num_cons"] = this->nCons;
    data["fitness"] = this->fitness;
    data["const_coeff"] = this->constraintCoeff;
    data["vars"] = this->variables;
    data["objs"] = this->objectives;
    data["cons"] = this->constraints;
    return data;
}

#endif  // DIGNEA_SOLUTION_H
