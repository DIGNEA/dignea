
# Create an algorithm 

[TOC]


To exemplify how to create a new algorithm in DIGNEA, we will see how to include the classical [Simulated Annealing](https://es.wikipedia.org/wiki/Algoritmo_de_recocido_simulado) algorithm.

## Creating the base Simulated Annealing class.

First of all, create a new **SimulatedAnnealing.h** file which will contain all the code for the SA algorithm.

To create a new algorithm and allow it to be usable across different methods and classes in DIGNEA, we must define the new algorithm as a subclass of AbstractEA in the **SimulatedAnnealing.h** file.  This class is the mother class for every type of algorithm that you may want to include in DIGNEA. Moreover, since DIGNEA is a template-based framework which aims to make algorithm as flexible as possible to address multiple types of problems and representations we must defines the *SimulatedAnnealing* class as a template-class with a template parameter to define the Solution type (SolutionTypes.h).

```cpp

#include <dignea/core/AbstractEA.h>

template <class S>
class SimulatedAnnealing : public AbstractEA<S> {
   // TODO: Include necessary methods and attributes
};
```

## Define the methods in SimulatedAnnealing

The next step is to consider the methods and attributes that our new algorithm must contain. Notice that it is also necessary to define the virtual methods from AbstractEA. It may be recomended as well to override some methods to define a custom operation. From AbstractEA we must define the following methods:

```cpp
public:
    void run() override;
    
    string getName() const override { return "Simulated Annealing"; };
    
    string getID() const override { return "SA"; }

    Front<S> getResults() const override;

protected:
    void initProgress() override;

    void updateProgress() override;

    void finishProgress() override;

    bool isStoppingConditionReached() override;

    void createInitialPopulation() override;

    void evaluatePopulation(vector<S> &vector1) override{};
```
First of all, the **run** method must be implemented for every new algorithm to define the custom workflow. Moreover, it is necessary to define the getName() and getID() methods. Besides, AbstractEA does not implement the getResults() method to allow algorithm-dependent results. 

Apart from this public methods, some protected methods must be overriden; i.e., the methods related to the evolutionary process (init, update, finish-Progress), the stop criteria checker method (isStoppingConditionReached) as well as the methods for generating and evaluating the population. Notice that the *evaluatePopulation* method is overriden as a empty method. The SA algorithm is a search-based metaheuristic which only uses a single individual as the solution to update in each generation. Therefore, the population size of this algorithm is equal to 1. We could still implement the *evaluatePopulation* and consider this restriction inside the method. However, we decide to show how to adapt the code for non population-based algorithms. The evaluation stage of the SA algorithm will be performed in the following *evaluateInds* method. This method will receive the current solution and a new offspring solution, evaluate both solutions and return the fittest solution to be set as our new current solution. 

```cpp
 S evaluateInds(S &current, S &offspring);
```

After that, we should include the basic attributes for a SA algorithm (*currentTemp* and *tempVariation*), and some extra methods to perform the basic steps of the [algorithm](https://es.wikipedia.org/wiki/Algoritmo_de_recocido_simulado#Pseudoc%C3%B3digo). The SimulatedAnnealing class at this stage is shown below.

```cpp
#include <dignea/core/AbstractEA.h>
#include <dignea/utilities/exceptions/NoMOAllowed.h> // For setProblem
#include <dignea/utilities/random/PseudoRandom.h>  // Random number generation
#include <vector>

using namespace std;

template <class S>
class SimulatedAnnealing : public AbstractEA<S> {
   public:
    SimulatedAnnealing(const int &maxEvaluations, const float &initialTemp,
                       const float &tempVariation);

    virtual ~SimulatedAnnealing() = default;

    void run() override;
   
    string getName() const override { return "Simulated Annealing"; };
    
    string getID() const override { return "SA"; }

    Front<S> getResults() const override;

    void setProblem(shared_ptr<Problem<S>> prob) override;

    void setProblem(Problem<S> *prob) override;
    
    float getTempVariation() const { return tempVariation; }
    
    void setTempVariation(float tempVariation) {
        SimulatedAnnealing::tempVariation = tempVariation;
    }
    
    float getCurrentTemp() const { return currentTemp; }
    
    void setCurrentTemp(float currentTemp) {
        SimulatedAnnealing::currentTemp = currentTemp;
    }

    void setPopulationSize(const int &pSize);

   protected:

    void initProgress() override;

    void updateProgress() override;

    void finishProgress() override;

    bool isStoppingConditionReached() override;

    void createInitialPopulation() override;

    void evaluatePopulation(vector<S> &vector1) override{};

    S evaluateInds(S &current, S &offspring);

   private:
    S applyPerturbations();

    void updateTemp() { currentTemp *= tempVariation; }

   private:
    float tempVariation;
    float currentTemp;
};
``` 

## Define the remaining methods.

Up to this point we have just define the basic methods signature and implement some inline methods. Now we should define the code inside the methods we have not defined yet.

### Constructor

The constructor for the SA algorithm must define the maximum number of evaluations to perform, the inital temperature and the temperature variation to perform at each generation. It is also mandatory to call the mother class constructor (AbstractEA).
Notice that the line *AbstractEA\<S\>(maxEvaluations, 1)* calls the AbstractEA constructor setting the maximum number of evaluations to perform and the population size to one.

```cpp
/**
 * @brief Construct a new Simulated Annealing object with all the parameters.
 *
 * @tparam S
 * @param maxEvaluations
 * @param initialTemp
 * @param tempVariation
 */
template <class S>
SimulatedAnnealing<S>::SimulatedAnnealing(const int &maxEvaluations,
                                          const float &initialTemp,
                                          const float &tempVariation)
    : AbstractEA<S>(maxEvaluations, 1),
      tempVariation(tempVariation),
      currentTemp(initialTemp) {}
```

### Run

The run method simply performs the SA algorithm. It starts by creating a initial population (one individual generated randomly), then while the maximum number of evaluations have not been reached yet it creates a new offspring from the current solution, evaluate both solutions and keeps the fittest one. After that it updates the current temperature.

```cpp
/**
 * @brief Runs the Simulated Annealing algorithm during maxEvaluations
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::run() {
    if (this->problem) {
        this->createInitialPopulation();
        this->startTime = chrono::system_clock::now();
        while (!this->isStoppingConditionReached()) {
            S offspring = applyPerturbations();
            this->population[0] = evaluateInds(this->population[0], offspring);
            updateTemp();
            this->updateProgress();
        }
        this->endTime = chrono::system_clock::now();
        this->finishProgress();
    } else {
        throw std::runtime_error("Problem is not set in SA::run");
    }
}
```

### Perturbation

This method simply transcribes the pseudocode of the SA for updating and existing solution.
Creates an offspring as a clone of the current solution (population[0]) and then perform some perturbations of each variable.

```cpp
/**
 * @brief Perturbation method of the SA algorithm. This method generates a new
 * offspring from the SA population (just one solution).
 *
 * @tparam S
 * @return S
 */
template <class S>
S SimulatedAnnealing<S>::applyPerturbations() {
    S offspring = this->population[0];
    double pert = PseudoRandom::randDouble();
    vector vars = offspring.getVariables();
    for (unsigned int i = 0; i < vars.size(); i++) {
        double newValue = pert * (this->problem->getUpperLimit(i) -
                                  this->problem->getLowerLimit(i)) +
                          this->problem->getLowerLimit(i);
        vars[i] = newValue;
    }
    offspring.setVariables(vars);
    return offspring;
}
```

### Population management

The following methods are designed to create and evaluate the solutions respectively. *createInitialPopulation* generates a single solution using the problem *createSolution* method and starts the evoluationary process. 

*evaluateInds* is the method that we defined to perform our custom evaluation (instead of using evaluatePopulation). This method receives two solutions, evaluates them and returns the new current solution based on SA selection criteria.

```cpp
/**
 * @brief Creates the initial population for the SA. The population of the SA is
 * just one individual.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::createInitialPopulation() {
    this->population[0] = this->problem->createSolution();
    this->problem->evaluate(this->population[0]);
    this->nextCheckpoint = 0;
    this->initProgress();
}

/**
 * @brief Evaluates the parent and offspring individuals at each generation.
 *
 * @tparam S
 * @param current
 * @param offspring
 * @return S
 */
template <class S>
S SimulatedAnnealing<S>::evaluateInds(S &current, S &offspring) {
    this->problem->evaluate(current);
    this->problem->evaluate(offspring);
    double diff = offspring.getFitness() - current.getFitness();
    if (diff < 0.0) {
        return offspring;
    } else {
        const double randProb = PseudoRandom::randDouble();
        const double e = exp(-diff / this->currentTemp);
        const double prob = min(1.0, e);
        return (randProb < prob) ? offspring : current;
    }
}
```

### Progress

The methods to manage the evolutionary process are straightforward. Init progress simply set the performed evaluations to one, update increases the evaluations in two and finally finishProgress computes the elapsed time.

```cpp
/**
 * @brief Initialises the evolutionary process. Sets the number of
 * performedEvaluations to 1.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::initProgress() {
    this->performedEvaluations = 1;
}

/**
 * @brief Updates the evolutionary process by increasing the
 * performedEvaluations in two.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::updateProgress() {
    this->performedEvaluations += 2;
}

/**
 * @brief Finishes the evolutionary process and computes the elapsed time.
 *
 * @tparam S
 */
template <class S>
void SimulatedAnnealing<S>::finishProgress() {
    std::chrono::duration<double> diff = this->endTime - this->startTime;
    this->elapsedTime = diff.count();
}

/**
 * @brief Checks whether the maximum number of evaluations have been performed
 *
 * @tparam S
 * @return true
 * @return false
 */
template <class S>
bool SimulatedAnnealing<S>::isStoppingConditionReached() {
    return (this->performedEvaluations >= this->maxEvaluations);
}
```

### Getters and Setters

Finally, the remaining getters and setters for SA are defined here. This methods although simple, required some condition evaluation to be performed safely. For instance, SA only works for single objective optimization problems and this must be checked in the setProblem methods. Moreover, the size of the population cannot be updated using the setPopulationSize method. 

```cpp
/**
 * @brief Returns the results obtained by the SA algorithm at the end of the
 * process.
 *
 * @tparam S
 * @return Front<S>
 */
template <class S>
Front<S> SimulatedAnnealing<S>::getResults() const {
    return Front<S>(this->population);
}
```

```cpp
/**
 * @brief Sets the problem to solve using Simulated Annealing.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void SimulatedAnnealing<S>::setProblem(shared_ptr<Problem<S>> prob) {
    if (prob->getNumberOfObjs() != 1) {
        std::string where = "SimulatedAnnealing setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem = prob;
}

/**
 * @brief Sets the problem to solve using Simulated Annealing.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void SimulatedAnnealing<S>::setProblem(Problem<S> *prob) {
    if (prob->getNumberOfObjs() != 1) {
        std::string where = "SimulatedAnnealing setProblem";
        throw(NoMOAllowed(where));
    }
    this->problem.reset(prob);
}

/**
 * @brief This methods is not implemented for this class. The population size of
 * a SA algorithm is always 1.
 *
 * @tparam S
 * @param prob
 */
template <class S>
void SimulatedAnnealing<S>::setPopulationSize(const int &pSize) {
    std::string where = "SimulatedAnnealing setPopulationSize";
    throw(NotImplemented(where));
}

#endif  // DIGNEA_SIMULATEDANNEALING_H

```