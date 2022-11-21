# Instance Generation Example

[TOC]

# Generating Knapsack Problem Instances with DIGNEA

Here's an example of how to generated diverse and discriminatory instances for the [Knapsack Problem](https://en.wikipedia.org/wiki/Knapsack_problem). 
We will be creating a set of KP instances which will be adapted to the performance of a specific Genetic Algorithm configuration. 
For this experiment we will be defining:

1. MEA's parameter configuration.
2. Bounds and size of the KP instances to generate.
3. The portfolio of algorithms to solve the instances. In this case, different configurations of GAs.
4. How to create a MEA object using the MEABuilder class.
5. Running the experiment.
6. Collecting the results in different instance files (".kp").

The full source code of this example is available at the Github repository insider the example directory. Link to file [here](https://github.com/amarrerod/dignea/blob/0d2028184cefd181279c3ebd9b0c6e3fcb6dfccf/examples/MEAKPExperiment.cpp).

## Parameter configuration

### MEA configuration
First of all, we will define the parameters for MEA. For a successfull run of MEA we need to specify the following parameters:

1. The number of generations to perform.
2. How many reps will the algorithm in the portfolio perform at each generation over every single instance.
3. The crossover and mutation rates for the genetic operators.
4. The number of instances to evolve (this is the "population size" of MEA).
5. How to evaluate the fitness of the instances. Here we will define EasyInstance to generate instances that are easy to solve by the target algorithm.
6. Fitness and diversity ratios for the WFF to compute the fitness.
7. Minimum diversity threshold to include a instance in the NoveltySearch archive.
8. Number of neighbours (k) to compute the diversity.
9. Distance metric to compute the diversity. In this case, Euclidean.
10. The Instance Generation Problem for MEA. For this example, IKPProblem since we are trying to generate KP instance.

### Knapsack Problem instances

We will also need to specify the minimum and maximum values that each profit and weight in the KP instance can take. 
In this experiment we will set the same bounds for both values, but you can set them independently. The bounds are [1, 1000].
Besides, we will set the instanceSize to be 100 so the KP instance will have N = 50 items.

```cpp
    // MEA Parameters
    auto generations = 1000; // Number of generations to evolve the instances
    // Number of repetitions each algorithm must solve every instance per generations
    auto reps = 10;          
    auto meaCXRate = 0.8;    // Crossover rate for MEA to apply the crossover on the instances
    auto nInstances = 10;    // Number of instances to evolve at each generation (population size)

    // How the fitness of the instances will be evaluated.    
    // In this case the instances must be easy to solve 
    // for the target
    auto easyEvaluator = make_unique<EasyInstances>(); 

     // KP instances parameters
    auto lowerBound = 1;    // Minimum value for the profits and weights
    auto upperBound = 1000; // Maximum value for the profits and weights
    // Size of the instance equals to 100.
    // Notice that in this case, the IKPSolution class stores the pairs (wi, pi)
    // for each item in the same vector. Therefore, setting instanceSize = 100
    // will generate instances of size N = 50 items.
    auto instanceSize = 100; 
    
    // Novelty Search parameters --> They are part of MEA as well
    auto fitnessRatio = 0.6f; // Weight of the fitness in the evaluation.
    auto diversityRatio = 0.4f; // Weight of the diversity in the evaluation.
    auto mutationRate = 1.0 / float(instanceSize); // Mutation rate for both MEA and algorithms.
    
    // Minimum difference in diversity for an instance to be included in the NS archive.
    auto thresholdNS = 50; 
    auto k = 3;  // Number of neighbours to compute during the NS.
    auto distance = make_unique<Euclidean<float>>(); // Distance metric for the NS
    // Instance Generation Problem --> IKPProblem.
    auto instKP = make_unique<IKPProblem>(instanceSize, nInstances, lowerBound,
                                          upperBound, lowerBound, upperBound);   

```

## Creating a portfolio of solvers

Now we have to define the portfolio of solvers to evaluate the instances in MEA. Remember that the order of
the algorithms in the portfolio is really important. The algorithm in the first position will be considered
the target and the instances will be biased to its performance.

We will use different four configurations of the ParallelGeneticAlgorithm for shaping this portfolio. All algorithms
will have the same configuration except for the crossover rate. The parameters we need to define are:

1. Maximum number of evaluations to perform.
2. Population size.
3. Crossover and mutation rates.
4. Crossover and mutation operators.
5. Selection operator.
6. Number of cores to use in parallel.

The portfolio must be a vector of unique_ptr to AbstractEA objects.  
For building such portfolio we encourage researchers to use the ParGABuilder (for ParallelGeneticAlgorithm) class since it make the building process more convenient.


```cpp
    
    // Algorithms parameters
    auto maxEvals = 100000; // Maximum evaluations to perform each algorith,
    auto popSize = 32;      // Population size of each algorithm. 
    // Different crossover rates to generate different configurations.
    vector crossRates = {0.7f, 0.8f, 0.9f, 1.0f}; 
    vector<unique_ptr<EA>> algorithms; // Portfolio of algorithms

    // Building four different GA configurations
    // Here we use a Parallel Genetic Algorithm to speedup the computation
    // The order of the algorithms in the portfolio is important, the
    // algorithm in the first position will be considered the target.
    for (int i = 0; i < 4; i++) {
        unique_ptr<ParallelGeneticAlgorithm<OS>> algorithm =
            ParGABuilder<OS>::create()
                .usingCores(32)
                .with()
                .mutation(MutType::UniformOne)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .populationOf(popSize)
                .withMutRate(mutationRate)
                .withCrossRate(crossRates[i])
                .runDuring(maxEvals);
        algorithms.push_back(move(algorithm));
    }
```

## Instantiate a MEA object

Now that we have define the parameters for MEA and created a portfolio, it is time to instantiate a MEA object using the MEABuilder class.
This class works in the same way that the ParGABuilder does. It receives the parameters we defined before and also allows the users to specify the
genetic operators and the novelty search variant on the fly. We strongly encourage users to instantiate MEA objects using this approach.

```cpp
    // Building the MEA with all the parameters
    unique_ptr<MEA<IP, IS, OP, OS>> mea =
        MEABuilder<IP, IS, OP, OS>::create()
            .toSolve(move(instKP))
            .with()
            .weights(fitnessRatio, diversityRatio)
            .portfolio(algorithms)
            .evalWith(move(easyEvaluator))
            .repeating(reps)
            .withSearch(NSType::Features, move(distance), thresholdNS, k)
            .with()
            .crossover(CXType::Uniform)
            .mutation(MutType::UniformOne)
            .selection(SelType::Binary)
            .withMutRate(mutationRate)
            .withCrossRate(meaCXRate)
            .populationOf(nInstances)
            .runDuring(generations);
```

## Run the experiment

Finally, we only have to call the **run** method from MEA and wait for the results. Notice that depending on the portfolio and other configuration values,
this process could take several hours (or even days). The parameters which has the most impact in the performance time of MEA are the portfolio, the number of generations,
the instance size and finally the number of instances to evolve.

Notice how we use the **to_json** methods from each object to get a JSON representation of the information. In addition, the InstPrinter class is used here to generate
a instance files with a "*.kp" extension for every instance in the set of solutions.

```cpp
    mea->run();

    // Getting the results in JSON
    auto meaData = mea->to_json();
    auto problemData = mea->getInstanceProblem()->to_json();
    
    auto front = mea->getResults();
    auto frontData = front.to_json();

    // You can also generate the .kp instance files associated to the solutions
    string kpInsPattern = "Inst_N_50_Target_GA_PS_32_CR_0.7_0.6f_0.4d";
    auto instPrinter = make_unique<InstPrinter<IS>>(kpInsPattern, ".kp");
    instPrinter->printInstances(front);

```

## Example of results

An example of the results obtained by DIGNEA is shown down below. Four different sets of KP instances were generated for different configurations of Genetic Algorithms using MEA.
![](../imgs/instances.png)


