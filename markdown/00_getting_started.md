# Getting Started

[TOC]

## How DIGNEA works.

DIGNEA (Diverse Instance Generator with Novelty Search and Evolutionary Algorithms) is a framework which allow researchers to generate diverse and discriminatory instances for any combinatorial optimization problem. The method uses a portfolio of solvers to evaluate its performance over a population of instances for a particular optimization problem. Moreover, it also uses a Novelty Search algorithm to search for instances that are diverse but also elicit discriminatory performance from a set of target solvers. A single run of DIGNEA per target solver provides discriminatory instances and broad coverage of the feature-space. 

### Main components.

The main building blocks of DIGNEA are the following classes:

1. AbstractEA: Abstract Evolutionary Algorithm, is the base algorithm interface used across DIGNEA. All new algorithm must be a subclass of it.
2. Problem: The Problem class defines a classical optimization to be solved (KP, TSP, Sphere, etc).
3. MEA: Meta-Evolutionary Algorithm. This class represents the instance generator algorithm. It receives a portfolio of solvers (one defined as the target algorithm) and it returns a set of diverse instances where the target solver outperforms the remaining algorithms in the portfolio; i.e., the instances are biased to the performance of such target. In addition, there are some classes strongly related to MEA:
   1. MEAProblem: This class represents a instance generation problem. This is used on MEA as the optimization problem to be solved.
   2. MEASolution: A MEASolution is a object which represents an instance for a particular optimization problem. The classes MEAProblem and MEASolution are completely dependent. This means, every problem has a solution type associated.
   3. NoveltySearch: The novelty search algorithm is used to evaluate the diversity of a instance. It cames to two differente version so far, NSFeatures and NSPerformance. The NSFeatures version uses a set of instance features to calculate the diversity of an instance in the features space. On the other hand, the NSPerformance variation only considers the diversity in the performance space of the portfolio.


### Results
An example of the results obtained by DIGNEA is shown down below. Four different sets of KP instances were generated for different configurations of Genetic Algorithms using MEA.
![](../imgs/instances.png)

### More information

**To get more information on how the method works check the following paper**: 

* Marrero, A., Segredo, E., León, C., Hart, E. (2022). A Novelty-Search Approach to Filling an Instance-Space with Diverse and Discriminatory Instances for the Knapsack Problem. In: Rudolph, G., Kononova, A.V., Aguirre, H., Kerschke, P., Ochoa, G., Tušar, T. (eds) Parallel Problem Solving from Nature – PPSN XVII. PPSN 2022. Lecture Notes in Computer Science, vol 13398. Springer, Cham. https://doi.org/10.1007/978-3-031-14714-2_16

