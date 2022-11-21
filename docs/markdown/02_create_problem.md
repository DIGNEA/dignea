
# Create a problem

[TOC]


This tutorial will show how to include a new optimization problem in DIGNEA. For this purpose, we will create a [Travelling Salesman Problem](https://en.wikipedia.org/wiki/Travelling_salesman_problem) class. The header and source files of this example are available at DIGNEA in TSP.h and TSP.cpp.


# Creating the Travelling Salesman Problem (TSP)

Let's start by creating a Travelling Salesman Problem class which will allow us to solve TSP instances. 

First, create a new file called **TSP.h** in your source directory. Then, you should include the necessary headers to extend from the Problem class. We will need the Problem.h, SolutionTypes.h files.
   
```cpp
#include <dignea/core/problem/Problem.h>
#include <dignea/types/SolutionTypes.h>
```

After that, we can start creating a new TSP class which will extend the Problem class using the IntFloatSolution representation. In this case, IntFloatSolution represents a solution which has:
      * A vector of integers as the representation for the solutions; i.e., the indeces of the cities to visit in order.
      * A float fitness value; i.e., the cost of doing a certain travel.

```cpp
#include <dignea/core/problem/Problem.h>
#include <dignea/types/SolutionTypes.h>

class TSP : public Problem<IntFloatSolution> {
  // TODO
};
```

## Defining the methods and attributes for TSP

This new TSP class must include at least the following methods inherited from Problem:

1. Default constructor.
2. Destructor.
3. Copy constructor.
4. Upper and lower bounds methods (4 and 5). This methods will determine the search space for each dimension of the optimization problem.
5. A getName method for identification (6).
6. The getOptimizationDirection method (7). This method will allow us to determinte whether the problem is a minimization or maxization problem. Notice that since multiple-objective is supported, it allows to defined diferent directions for each objective. The method must return one of the Minimize or Maximize constants.
7. The createSolution methods (8, 9). This methods will generate and return a new individual for solving the problem. This method is used by the EAs to generate a population. The difference between each method is the used of a ParallelPRNG instance to generate random numbers.
8. The evaluation methods. In particular, an evaluate and evaluateConstraints methods. This methods are self-explanatory. They take an individual and then evaluate then with respect to the problem to solve. Evaluate (10) uses the method "setFitness" from the indiviudal to update the fitness and "evaluateConstraints" (11) return whether the individual is feasible or not.
9. We also need to include a readData method so the problem can retrieve information from the filesystem or other sources.
10. Finally, we must include a to_json() method which returns a json representation of the problem instance.

Notice that you can also include extra methods and variables as necessary. In this case, we included a vector of coordinates representing the points and a matrix of distances, and extra methods for creating a TSP object. Moreover, we defined a new type called coords which represents the (x, y) coordinates of a city in a TSP instance.

The full TSP.h file detailed below and the implementation of these methods are included in a TSP.cpp file.


```cpp

/**
 * Pair of floats to represent point coordinates (x, y)
 **/
using coords = std::pair<float, float>;

/**
 * Class representing the TSP problem
 */
class TSP : public Problem<IntFloatSolution> {
   public:
    TSP(); // (1)

    virtual ~TSP() = default; // (2)

    explicit TSP(const int &numberOfVars, const vector<coords> &coordinates);

    TSP(TSP const *); // (3)

    float getUpperLimit(int i) const override; // (4)

    float getLowerLimit(int i) const override; // (5)

    string getName() const override { return "TSP"; }; // (6)

    int getOptimizationDirection(const int i) const override; // (7)

    IntFloatSolution createSolution() const override; // (8)

    IntFloatSolution createSolution(ParallelPRNG &engine) const override; // (9)

    void evaluate(IntFloatSolution &solution) const override; // (10)

    bool evaluateConstraints(IntFloatSolution &solution) const override; // (11)

    void readData(const string &path) override; // (12)

    json to_json() const override; // (13)

  protected:
    void computeDistances();

  protected:
    vector<coords> points;
    vector<vector<float>> distances;
};
```

## TSP implementation in TSP.cpp

Now, create a TSP.cpp file to implement all the methods defined in the previous section. First, make sure to include the header file (TSP.h) in the new TSP.cpp as: 

```cpp

#include "relative_path_to_file/TSP.h"
```

Then we continue our implementation.

### Constructors

The constructors for the TSP class are considerably simple.  First, the default constructor will create an optimization problem with dimension zero, one objective and one constraint.

```cpp
TSP::TSP()
    : Problem<IntFloatSolution>(0, 1, 1),
      pathToInstance(""),
      points(),
      distances() {}
```

We define three more constructors to create a TSP class defining only the number of variables to consider. This works similar to the previous constructor but with dimension != 0.

```cpp
TSP::TSP(const int &numberOfVars)
    : Problem(numberOfVars, 1, 1), pathToInstance(""), points(), distances() {}
```

Another constructor with the number of variables in the instance and a vector of coordinates to define the cities. In this case, we have the dimension and the cities so we can compute the distance matrix **D** and solve the problem.

```cpp
TSP::TSP(const int &numberOfVars, const vector<coords> &coordinates)
    : Problem(numberOfVars, 1, 1), pathToInstance(""), points(coordinates) {
    computeDistances();
}
```

The TSP constructor which receives a string with a filename to a TSP instance uses this information to create the class. It reads all the data from the file (dimension, coordinates, etc) and set all the variables.

```cpp
TSP::TSP(const string &path)
    : Problem<IntFloatSolution>(0, 1, 1),
      pathToInstance(path),
      points(),
      distances() {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading TSP instance.";
    try {
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars;
        this->points.reserve(numberOfVars);
        float x, y;
        while (inputFile >> x >> y) {
            points.push_back(std::make_pair(x, y));
        }

        inputFile.close();
    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
    computeDistances();
}
```

Finally we have the copy constructor which uses another previously create TSP object to generate a new one.

```cpp
TSP::TSP(TSP const *tsp) : Problem<IntFloatSolution>() {
    this->numberOfVars = tsp->numberOfVars;
    this->numberOfObjs = tsp->numberOfObjs;
    this->numberOfCons = tsp->numberOfCons;
    this->pathToInstance = tsp->pathToInstance;
    this->points = tsp->points;
    this->distances = tsp->distances;
}
```

### Auxiliary building methods

To properly build a TSP object, we could also need other methods such as readData or computeDistances. 
The first, works as the TSP(string) constructor by reading the information from a file and setting all the variables accordingly. 
This method is useful when we use the default constructors.

On the other hand, the computeDistance methods is in charge of computing the **D** matrix with all the distances between each pair of cities
in the instance. Notice that we consider the instance to be symmetric (Dij = Dji).

```cpp
void TSP::readData(const string &path) {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading TSP instance.";
    try {
        this->pathToInstance = path;
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars;
        this->points.reserve(numberOfVars);
        float x, y;
        while (inputFile >> x >> y) {
            points.push_back(std::make_pair(x, y));
        }
        inputFile.close();
    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
    computeDistances();
}

void TSP::computeDistances() {
    distances.resize(this->numberOfVars,
                     vector<float>(this->numberOfVars, 0.0f));
    for (int i = 0; i < this->numberOfVars; i++) {
        for (int j = i + 1; j < this->numberOfVars; j++) {
            distances[i][j] = ((points[j].first - points[i].first) *
                               (points[j].first - points[i].first)) +
                              ((points[j].second - points[i].second) *
                               (points[j].second - points[i].second));
            distances[j][i] = distances[i][j];
        }
    }
}
```

### Create new random solutions

To create new solution for the TSP instance, we must define the createSolution method. This method must randomly generate a valid TSP solution and return it.
It is really important to note that the **dimension of the IntFloatSolution returned is equal to the problem dimension plus one**. Since the TSP version that
we are considering here must start from city 0 and return to it at the end, the solution must have an extra variable at the end equal to zero. 
Moreover, a solution it is only considered to be feasible if it fulfils this contraint.

```cpp
IntFloatSolution TSP::createSolution() const {
    IntFloatSolution solution(this->numberOfVars + 1, 1, 1);
    vector<int> vars(this->numberOfVars + 1);  // For extra the zero at the end
    std::iota(std::begin(vars), std::end(vars) - 1, 0);
    vars[vars.size() - 1] = 0; // Zero at the end
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(vars) + 1, std::end(vars) - 1, rng); // Random from 1...n-2
    solution.setVariables(vars);
    return solution;
}
```

### Evaluation

The evaluation of a TSP solution is done in two steps. 
First, we evaluate the feasibility of the solution by checking that each city only appears one in the solutions and that 
the travel starts and ends in the city number zero. This is evaluate in the evaluateConstraints method.

```cpp
bool TSP::evaluateConstraints(IntFloatSolution &solution) const {
    std::set<int> inSolution;
    vector<int> vars = solution.getVariables();
    const unsigned int size = vars.size();
    if (vars[0] != 0 || vars[vars.size() - 1] != 0) {
        return false;
    }
    inSolution.insert(0);
    for (unsigned int i = 1; i < size - 1; i++) {
        if (inSolution.count(vars[i]) != 0) {
            return false;
        } else {
            inSolution.insert(vars[i]);
        }
    }

    return true;
}
```

If the solution is feasible, we then compute the sum of the distance of this travel using the distance matrix **D**. 
Otherwise, we set a penalty and distance values equal to the maximum float allowed.

Finally, the fitness of the solution is set as $f = 1.0 / distance$. Therefore, the shorter the path, the higher the fitness of a solution.
It is important to set the value of the objectives and constraints in case our algorithm needs this information as well. The objective value 
is set to the distance of the travel and the constraintCoeff and the contraint value is the penalty value.

```cpp
void TSP::evaluate(IntFloatSolution &solution) const {
    if (this->distances.empty()) {
        throw runtime_error(
            "Cannot evaluate solution in TSP since distance vector is empty.");
    }
    if (solution.getNumberOfVars() != this->getNumberOfVars() + 1) {
        throw runtime_error(
            "Cannot evaluate solution in TSP since dimensions are different. "
            "Dimension is: " +
            to_string(this->getNumberOfVars() + 1) +
            " and the individual has dimension: " +
            to_string(solution.getNumberOfVars()));
    }
    float distance = 0.0;
    float penalty = 0.0;
    if (evaluateConstraints(solution)) {
        penalty = 0;
        vector<int> variables = solution.getVariables();
        const unsigned int size = variables.size();
        for (unsigned int i = 0; i < size - 1; i++) {
            distance += this->distances[variables[i]][variables[i + 1]];
        }

    } else {
        penalty = std::numeric_limits<float>::max();
        distance = std::numeric_limits<float>::max();
    }
    float fitness = 1.0 / distance;
    solution.setFitness(fitness);
    solution.setObjAt(0, distance);
    solution.setConstraintCoeff(penalty);
    solution.setConstraints({penalty});
}
```


### Getters

The bounds and optimization direction for TSP are straightforward. The upper limit of the problem
is consider to be the number of cities minus one and the lower limit is one since the zero is only
for starting and finishing the travel. Moreover, TSP is a minimization problem and therefore the getOptimizationDirection 
method always returns Minimize.

```cpp
float TSP::getUpperLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getUpperLimit in TSP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return this->numberOfVars - 1;
}

float TSP::getLowerLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getLowerLimit in TSP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 1;
}

int TSP::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= this->getNumberOfObjs()) {
        std::string where =
            "getOptimizationDirection in TSP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}
```
### JSON

Finally, the to_json() method returns a json representation of the TSP instance. 
Notice that we call super the Problem::to_json() method for retreiving the mother class information as well.

```cpp
json TSP::to_json() const {
    json data = Problem<IntFloatSolution>::to_json();
    data["points"] = this->points;
    data["distances"] = this->distances;
    return data;
}
```