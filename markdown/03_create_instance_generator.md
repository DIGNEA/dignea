

# Create a instance generation problem

[TOC]

This tutorial will show how to include a new instance generation problem in DIGNEA. For this purpose, we will create a new TSP instance generator using the code from how to create an [optimization problem](02_create_problem.md). To create a new instance generation problem we must define at least three classes. The optimization problem underneath (TSP in this case), a new optimization problem class for MEA (MEAProblem), and a solution class for instance generation problem which represent a instance of the optimization problem itself (MEASolution). For this tutorial the classes are as follows:

1. TSP: The optimization problem we want to generate instances for.
2. ITSPProblem: Instance TSP Problem. The problem that will pass to MEA.
3. ITSPSolution: A solution for the ITSPProblem. A ITSPSolution contains the information of a TSP instance. 

Moreover, since MEA uses different approaches of Novelty Search (NSFeatures and NSPerformance) it may also be necessary to define the features that represent an instance of the optimization problem. If you plan to use the NSFeatures version, you definetively must define the features. Thus, you may also need to consider how to compute this features and implement the corresponding algorithms. For the TSP, there exist lots of related literature and we will use some well-known features:

1. Size of the instance (number of cities).
2. Mean distance between cities.
3. Standard deviation of the distance between cities.
4. The coordinates of the centroid of the map (Cx,Cy).
5. The radius of the instance.
6. From the DBSCAN algorithm: the cluster ratio, cluster radius and fraction of distance.



## Creating the Travelling Salesman Problem (TSP)

First of all we should define the optimization problem for which the instance will be generated for.
Since we are using the TSP, just go to the [how to create a problem setion](02_create_problem.md).


## Create the new MEAProblem subclass: ITSPProblem class.

**NOTE:** For the sake of simplicity, we will combine the code from ITSPProblem.h and ITSPProblem.cpp in the same page. However, note that you should split the code appropiately.

First, the ITSPProblem must be defined as a subclass of the MEAProblem class. Since this is a specialization of the 
template, we can defined the exact template parameters that we will use. In this case, MEAProblem expects two template parameters:

* The Optimization Problem underneath: TSP.
* The MEASolution class associated: ITSPSolution (to be created).

We should include the private attributes and methods for computing the features of a TSP instance (computeCentroid, instanceRadius, distinctDistance, eps, minNeighbors).
Besides, the upper and lower limits for the coordinates could be useful too. This values will be used to defined the maximum and minimum values of (x, y) for a city in the instances.

```cpp

#include <dignea/mea/MEAProblem.h>
#include <dignea/mea/solutions/ITSPSolution.h>
#include <dignea/problems/TSP.h>
#include <dignea/types/SolutionTypes.h>

class ITSPProblem : public MEAProblem<TSP, ITSPSolution> {
   public:
    // TODO
    
   private:
    coords computeCentroid(const vector<coords> &points);

    float instanceRadius(const vector<coords> &points, const coords &centroid);

    float distinctDistances(const vector<vector<float>> &distances);

   private:
    float upperLimit;
    float lowerLimit;
    float eps;
    int minNeighbors;
};
```

After that, we can start to defined the methods that we will implement in ITSPProblem. Most of then are inherited from MEAProblem.

### Constructors 

The constructors will allow us to create new ITSPProblem objects. Notice that in some constructors we are passing some parameters for the configuration
of the DBSCAN algorithm. This is necessary for using the NSFeatures. The information received in the constructors is treated as follows:

1. The number of variables (dimension) should be set as the double of the number of variables which it is used to create an object. This is because the ITPSolution must be twice size of the nVars since it stores the pairs (x,y) coordinates for each city.
2. minL and maxL are the bounds for the coordinates. The default limits for the coordinates are [0, 1000].
3. eps is the minimum distance to a city to be consider a neighbour (the default value is 5.0).
4. minNeighbors is the minimum number of neighbours to consider a cluster (default = 2).

```cpp
ITSPProblem::ITSPProblem()
    : MEAProblem<TSP, ITSPSolution>(1, 1, 0),
      upperLimit(1000),
      lowerLimit(0),
      eps(5.0),
      minNeighbors(2) {}

ITSPProblem::ITSPProblem(const int &numberOfVars)
    : MEAProblem<TSP, ITSPSolution>(numberOfVars, 1, 0),
      upperLimit(1000),
      lowerLimit(0),
      eps(5.0),
      minNeighbors(2) {}

ITSPProblem::ITSPProblem(const int &numberOfVars, const float &maxL,
                         const float &minL, const float e, const int minN)
    : MEAProblem<TSP, ITSPSolution>(numberOfVars, 1, 0),
      upperLimit(maxL),
      lowerLimit(minL),
      eps(e),
      minNeighbors(minN) {}

ITSPProblem::ITSPProblem(const ITSPProblem *other)
    : MEAProblem<TSP, ITSPSolution>(other->numberOfVars, 1, 0) {
    upperLimit = other->upperLimit;
    lowerLimit = other->lowerLimit;
    eps = other->eps;
    minNeighbors = other->minNeighbors;
}
```

### Generate Optimization Problem from Instance Solution

This method simply gets a ITSPSolution object and constructs a TSP object with all the information related.
It returns a shared_ptr since this object will be used by several algorithms in MEA and none of them will own the object.

```cpp
    /// @brief Generates a TSP problem with the information in the ITSPSolution
    /// to be solved in the MEA by the algorithms in the portfolio.
    /// @param instance
    /// @return
    shared_ptr<TSP> genOptProblem(const ITSPSolution &instance) const override {
        vector<coords> variables = instance.to_coords();
        if (instance.getVariables().size() != (unsigned int)this->numberOfVars) {
            string where = "instance.variables.size() != Problem inner dimension";
            throw OutOfRange(where);
        }
        return make_shared<TSP>(variables.size(), variables);
    };

```
### Create new solutions

We define three methods to generate new solutions in the ITSPProblem. The first one, will create a vector of size maxSolution with ITSPSolution objects each of them representing a new TSP instance. Each ITSPSolution object is created by defining the number of variables (dimension) and a vector of randomly generated floats. Notice that this vector actually encodes a TSP instance representation where the coordinates (x, y) are stored in the even and odd positions of the vector respectively: [x0, y0, x1, y1, ..., xn-1, yn-1].

The other two methods will generate only one solution at a time following a similar procedure.

```cpp
    vector<ITSPSolution> createSolutions(
        const int &maxSolutions) const override {
        vector<ITSPSolution> solutions;
        solutions.reserve(maxSolutions);
        for (int i = 0; i < maxSolutions; i++) {
            vector<float> vars;
            vars.reserve(this->numberOfVars);
            for (int j = 0; j < this->numberOfVars; j++) {
                float x =
                    PseudoRandom::randDouble(this->lowerLimit, this->upperLimit);
                vars.push_back(x);
            }
            ITSPSolution solution(this->numberOfVars);
            solution.setVariables(vars);
            solutions.push_back(solution);
        }
        return solutions;
    }

    /// @brief Creates a single solution (TSP instance)
    /// @return
    ITSPSolution createSolution() const override;

    /// \deprecated @brief Creates a single solution (TSP instance)
    /// @param engine
    /// @return
    ITSPSolution createSolution(ParallelPRNG &engine) const override;
```

### Evaluation and feature calculation

MEAProblem are not meant to use the evaluation methods. However, we must define them anyway since they are subclasses of Problem. From Problem, we must define **evaluate** and
**evaluateConstraints** to do nothing. From MEAProblem, we must implement **beforeEvaluation** and **afterEvaluation**. This methods are quite useful to perform some ad-hoc computation to the ITSPSolutions before and after trying to solve the instances
with the algorithms from the portfolio (MEA).

For ITSPSolutions, we do not need to perform any calculation before evaluating the instances. However, after evaluting the instances we should compute the set of features
for each of them. This is done in the **afterEvaluation** method. For each instance (ITSPSolution) we use some private methods and DBSCAN to calculate the set of features that we mentioned at the begining of this tutorial:

1. Size of the instance (number of cities).
2. Mean distance between cities.
3. Standard deviation of the distance between cities.
4. The coordinates of the centroid of the map (Cx,Cy).
5. The radius of the instance.
6. From the DBSCAN algorithm: the cluster ratio, cluster radius and fraction of distance.

Then, we create a vector of floats called features with all these values and use the **setFeatures** method of the ITSPSolution class to update the features.

```cpp
    void evaluate(ITSPSolution &solution) const override { return; };

    bool evaluateConstraints(ITSPSolution &solution) const override {
        return true;
    };

    void beforeEvaluation(vector<ITSPSolution> &solutions) override { return; };

    void afterEvaluation(vector<ITSPSolution> &solutions) override {
        for (ITSPSolution &solution : solutions) {
        vector vars = solution.getVariables();
        auto distances = this->genOptProblem(solution)->getDistances();
        auto points = solution.to_coords();

        DBSCAN dbscan(minNeighbors, eps, points);
        auto nClusters = dbscan.run();
        float clusterRatio = -1.0f;
        float clusterRadius = -1.0f;
        if (nClusters != 0) {
            clusterRatio = vars.size() / nClusters;
            clusterRadius = dbscan.radiusOfClusters();
        }
        if (clusterRatio == std::numeric_limits<float>::infinity() ||
            isnan(clusterRatio)) {
            clusterRatio = -1.0f;
        }
        if (isnan(clusterRadius)) {
            clusterRadius = -1.0f;
        }
        coords centroid = computeCentroid(points);
        float radius = instanceRadius(points, centroid);
        float fractionDist = distinctDistances(distances);
        float meanDist = mean(distances);
        float stdDist = standardDev(meanDist, distances);

        vector<float> features = {
            (float)vars.size(), meanDist,        stdDist,
            centroid.first,     centroid.second, radius,
            clusterRatio,       clusterRadius,   fractionDist};

        solution.setFeatures(features);
        }
    };

```

### Getters and Setters

Finally, the getters and setters for this class are as simple as other optimization problem.

```cpp
    /// @brief Returns the optimization direction for each dimension. Minimize
    /// in each case.
    /// @param i
    /// @return
    int getOptimizationDirection(const int i) const override {
        if (i < 0 || i >= this->getNumberOfObjs()) {
        std::string where =
            "getOptimizationDirection in ITSProblem with index = " +
            to_string(i);
        throw(OutOfRange(where));
        }
        return Minimize;
    }

    /// @brief Get the problem name
    /// @return a String with the information
    string getName() const override { return "ITSPProblem"; }

    /// @brief Get the upper limit of the ith dimension (default = 1000)
    /// @param i
    /// @return
    float getUpperLimit(const int i) const override {
        if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getUpperLimit in ITSP with index = " + to_string(i);
        throw(OutOfRange(where));
        }
        return upperLimit;
    }

    /// @brief Get the lower limit of the ith dimension (default = 0)
    /// @param i
    /// @return
    float getLowerLimit(const int i) const override {
        if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getLowerLimit in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
        }
        return lowerLimit;
    }

    /// @brief Set the upper limit for the ith dimension
    /// @param l
    void setUpperLimit(const float &l) { this->upperLimit = l; }

    /// @brief Set the lower limit for the ith dimension
    /// @param l
    void setLowerLimit(const float &l) { this->lowerLimit = l; }

    /// @brief Set the epsilon parameter for the DBSCAN algorithm
    /// @param e
    void setEPS(const float e) { eps = e; }

    /// @brief Get the epsilon parameter of the DBSCAN algorithm
    /// @return
    float getEPS() const { return eps; }

    /// @brief Set the minimum number of neighbors to consider a cluster in the
    /// DBSCAN algorithm
    /// @param m
    void setMinNeighbors(const int m) { minNeighbors = m; }

    /// @brief Get the minimum number of neighbors to consider a cluster in the
    /// DBSCAN algorithm
    /// @return
    int getMinNeighbors() const { return minNeighbors; }

    /// @brief Not implemented here.
    /// @param path
    void readData(const string &path) override{};

```


## Create the solution for the instance generation problem: ITSPSolution class.

A ITSPSolution object is solution for the ITSPProblem class. This class is considerably simpler than the previous one. Here we only need to define some constructors, getters and setters and some representation methods. As before, the code shown here must be splitted appropiately in a ITSPSolution.h and a ITSPSolution.cpp files.

```cpp
#include <dignea/mea/MEASolution.h>

#include <utility>

using coords = std::pair<float, float>;

/**
 * @brief Instance Traveling Salesman Problem Solution.
 * Class which represents the solution for the Instance Traveling Salesman
 * Problem (ITSPProblem).
 *  The variables inherited from the Solution<float, float> class contains the
 * pairs of coordinates for each point (city) in the instance.
 *      [(x0,y0), ..., (xn-1,yn-1)] in a single array.
 *
 */
class ITSPSolution : public MEASolution<float, float> {
   public:
   // TODO
};
```

### Constructors

The constructors for ITSPSolution class only need to call the super MEASolution constructor to build the object. 
Notice that passing nVars = 100 will create a 50 cities since the number of variables must be divided by two to contain the coordinates for each city.

```cpp
    ITSPSolution::ITSPSolution() : MEASolution<float, float>() {}

    ITSPSolution::ITSPSolution(const int &nVars)
        : MEASolution<float, float>(nVars, 1, 0) {}

    ITSPSolution::ITSPSolution(const ITSPSolution &solution)
        : MEASolution<float, float>(solution) {}

    ITSPSolution::ITSPSolution(const ITSPSolution *solution)
        : MEASolution<float, float>(solution) {}
```

### Representation 

Finally, the following methods will get a new representation of the TSP instance (a ITSPSolution object) in JSON, in a instance file format or as a vector of coordinates.

to_json will create and fill a new json object with all the necessary information. We must include the features, the number of variables, the coords of the cities (so we can construct the actual TSP instance later), the diversity, fitness and biasedFitness values.

to_instance method is useful for generating the instance file of each ITSPSolution object. This is the method that InstPrinter uses to generate the files.

```cpp
    /// @brief Creates and returns a JSON object with the information from the
    /// TSP.
    /// @return
    json to_json() const override {
        json data;
        data["n_vars"] = this->nVars;
        data["coords"] = this->variables;
        data["diversity"] = this->diversity;
        data["biasedFitness"] = this->biasedFitness;
        data["fitness"] = this->fitness;
        vector keys = {
            "size",   "mean_dist",     "std_dist",       "c_x",          "c_y",
            "radius", "cluster_ratio", "cluster_radius", "fraction_dist"};
        vector values = this->getFeatures();
    
        map<string, float> featuresMap;
        auto zip = [](const std::string &s, float f) {
            return std::make_pair(s, f);
        };
        transform(keys.begin(), keys.end(), values.begin(),
                inserter(featuresMap, featuresMap.end()), zip);
        data["features"] = featuresMap;
        return data;
    };

    /// @brief Generates the vector of coordinates for the instance. Look the
    /// TSP getPoints() method.
    /// @return
    vector<coords> to_coords() const {
         vector<coords> coordinates;
        coordinates.reserve(this->variables.size());
        for (unsigned int i = 0; i < this->variables.size() - 1; i += 2) {
            coordinates.push_back({variables[i], variables[i + 1]});
        }
        return coordinates;
    };

    /// @brief Generates the instance representation of the TSP and puts it in
    /// the ostream
    /// @param os
    virtual void to_instance(std::ostream &os) const {
        os << this->getNumberOfVars() / 2 << std::endl;
        vector vars = this->getVariables();
        for (unsigned int i = 0; i < vars.size() - 1; i += 2) {
            os << vars[i] << " " << vars[i + 1] << std::endl;
        }
    }
};
```

The result of to_instance is something like the following:

```bash
100 # Number of cities
1380 939 # x0 y0
2848 96  # x1 y1
3510 1671 # x2 y2
457 334
3888 666
984 965
2721 1482
1286 525
2716 1432
738 1325
1251 1832
2728 1698
3815 169
3683 1533
1247 1945
123 862
1234 1946
252 1240
611 673
2576 1676
928 1700
53 857
1807 1711
274 1420
2574 946
178 24
2678 1825
1795 962
3384 1498
3520 1079
1256 61
1424 1728
3913 192
3085 1528
2573 1969
463 1670
3875 598
298 1513
3479 821
2542 236
3955 1743
1323 280
3447 1830
2936 337
1621 1830
3373 1646
1393 1368
3874 1318
938 955
3022 474
2482 1183
3854 923
376 825
2519 135
2945 1622
953 268
2628 1479
2097 981
890 1846
2139 1806
2421 1007
2290 1810
1115 1052
2588 302
327 265
241 341
1917 687
2991 792
2573 599
19 674
3911 1673
872 1559
2863 558
929 1766
839 620
3893 102
2178 1619
3822 899
378 1048
1178 100
2599 901
3416 143
2961 1605
611 1384
3113 885
2597 1830
2586 1286
161 906
1429 134
742 1025
1625 1651
1187 706
1787 1009
22 987
3640 43
3756 882
776 392
1724 1642
198 1810
3950 1558 # x99 y99
```