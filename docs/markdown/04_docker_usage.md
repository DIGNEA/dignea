# DIGNEA through Docker containers

[TOC]

## How to download DIGNEA from Docker Hub

For downloading the DIGNEA image from Docker Hub you just simply run the following command:

```bash
docker pull dignea/dignea
```

This will download the image and then you will be able to run a container with it. It is important that you have a working installation of Docker in your computer as well as checking that Docker is actually runing on your computer while you execute the previous command. If you need information on how to install Docker, please check the official documentation [here](https://docs.docker.com/engine/install/).

## How to run a container with DIGNEA

Once you have the DIGNEA image in your local computer, the next step is to run a container with it. For this purpose, the only command you need is the following. This command will create the container using the DIGNEA image and it will locate you inside the "/dignea" directory.

```bash
docker run -it dignea/dignea:latest
```

Running the "ls" command inside should give you an overview of the directories in dignea. Something like: 

```bash
root@4c8b3b99fb5a:/dignea# ls
CMakeLists.txt      Dockerfile  License.md  bin    build.sh     cppcheck_report.txt  docs      include  scripts  test
CodeCoverage.cmake  Doxyfile    README.md   build  codecov.yml  data                 examples  lib      src
root@4c8b3b99fb5a:/dignea# 
```

## How to build DIGNEA

Once you start a container using DIGNEA image, you will find yourself inside the dignea/. It is not necessary to build DIGNEA from the start, but, if you create more experiments or update the software in any way, you should re-build the framework to include the changes. For that, just run the following command inside the container:

```bash 
 ./build.sh
```

## Examples and tests

Inside the *dignea/bin* directory you will find the tests (digneaTest) and some examples of the framework. Notice that some examples such as KP_HEURISTICS or MEAs experiments will take lots of time to run.

### Test suite

digneaTest is the executable which runs the test suite of DIGNEA. Its job is to check that everything is working fine. This executable does not produce any new file or data, only checks that every function and class of the software works as expected. The output of this executable should be something like the following:

```sh
Run tests
Randomness seeded to: 3381122180
===============================================================================
All tests passed (461757 assertions in 69 test cases)

[100%] Built target digneaTest
```

### Generating KP instances

We include to executables to generate KP instances. One uses four deterministic heuristics and the other four different configurations of EAs.

####  KP_HEURISTICS 

Generates sets of biased instances to four different KP heuristics. The results of the execution includes JSON files with information of the runs and KP instance files. This is a computational expensive experiment which uses the number of repetitions as a parameter to reduce the computation time. 

This file receives the number of repetitions to perform as parameter. Then, it generates a set of diverse instances for each KP heuristic in DIGNEA at this point. The heuristics are:

* 1. Default Heuristic
* 2. Maximum Profit per Weight Ratio Heuristic
* 3. Maximum Profit Heuristic
* 4. Minimum Weight Heuristic

The expected output is a JSON file for each solver and repetition. For instance, setting the number of repetitions parameter to 10 will produce 40 JSON file (10 for each solver). The structure of the JSON file is similar to the following. Please note that some information is omitted for a clearer visualisation.

```json
{
    "algorithm": {
        "portfolio": [
            {
                "isTarget": true,
                "name": "Default KP"
            },
            {
                "isTarget": false,
                "name": "MPW KP"
            },
            {
                "isTarget": false,
                "name": "MaP KP"
            },
            {
                "isTarget": false,
                "name": "MiW KP"
            }
        ],
        "crossover": "Uniform One Crossover",
        "crossover_rate": 0.800000011920929,
        "elapsed_time": 2.0608113610000003,
        "max_generations": 1000,
        "mutation": "Uniform One Mutation",
        "mutation_rate": 0.009999999776482582,
        "name": "EIG",
        "novelty_search": {
            "k": 3,
            "name": "NSFeatures",
            "threshold": 3.0
        },
        "pop_size": 10,
        "repetitions": 10,
        "weighted_function": {
            "fitness_ratio": 0.4000000059604645,
            "novelty_ratio": 0.6000000238418579
        }
    },
    "front": {
        "0": {
            "biasedFitness": 90.0,
            "capacity": 7154,
            "conf_fitness": [
                [
                    8728.0,
                    8728.0,
                    8728.0,
                    8728.0,
                    8728.0,
                    8728.0,
                    8728.0,
                    8728.0,
                    8728.0,
                    8728.0
                ],
                [
                    7500.0,
                    7500.0,
                    7500.0,
                    7500.0,
                    7500.0,
                    7500.0,
                    7500.0,
                    7500.0,
                    7500.0,
                    7500.0
                ],
                [
                    8628.0,
                    8628.0,
                    8628.0,
                    8628.0,
                    8628.0,
                    8628.0,
                    8628.0,
                    8628.0,
                    8628.0,
                    8628.0
                ],
                [
                    8638.0,
                    8638.0,
                    8638.0,
                    8638.0,
                    8638.0,
                    8638.0,
                    8638.0,
                    8638.0,
                    8638.0,
                    8638.0
                ]
            ],
            "diversity": 3225.72607421875,
            "features": {
                "Q": 7154.0,
                "avg_eff": 0.550000011920929,
                "max_p": 993.0,
                "max_w": 967.0,
                "mean": 484.6400146484375,
                "min_p": 22.0,
                "min_w": 21.0,
                "std": 274.043212890625
            },
            "fitness": 1971.4356689453125,
            "n_vars": 100,
            "profits": [
                424,
                746,
                583,
                75,
                600,
                703,
                861,
                257,
                907,
                908,
                758,
                537,
                451,
                872,
                640,
                398,
                237,
                53,
                327,
                111,
                161,
                456,
                328,
                170,
                515,
                272,
                435,
                993,
                513,
                872,
                885,
                293,
                253,
                401,
                472,
                105,
                394,
                401,
                103,
                459,
                360,
                623,
                514,
                59,
                347,
                128,
                22,
                826,
                381,
                42
            ],
            "weights": [
                544,
                829,
                564,
                645,
                180,
                490,
                303,
                711,
                572,
                842,
                649,
                217,
                663,
                634,
                21,
                611,
                321,
                32,
                663,
                118,
                368,
                929,
                96,
                105,
                862,
                701,
                398,
                600,
                363,
                905,
                937,
                746,
                214,
                493,
                450,
                72,
                967,
                818,
                585,
                469,
                553,
                509,
                842,
                664,
                634,
                458,
                87,
                877,
                846,
                76
            ]
        },
        "n_solutions": 1
    },
    "problem": {
        "low_limit": 1.0,
        "name": "EKKProblem",
        "num_cons": 0,
        "num_objs": 1,
        "num_vars": 100,
        "up_limit": 1000.0
    }
}
```

#### EIGKPExperiment: 

Generates sets of biased instances to four different GA configurations. The results of the execution includes JSON files with information of the runs and KP instance files. This is a computational expensive experiment which can take lots of time to finish (even a day depending on the computational resources). The expected parameters are:  
  - Four different crossover rates for the GA configurations separated by a space.
  - A fitness ratio between 0 and 1
  - A novelty ratio between 0 and 1.

**NOTE: The fitness and novelty ratios must sum-up to 1.0**

The expected output is a single JSON file for the given configuration. The structure of the JSON file is similar to the previous example. 

Moreover, this executable will produce a KP instance file (.kp) for each instance generated during the process. The results will similar format to the data include inside "dignea/data/instances/KnapsackProblem/**/*.kp" directory. Although this files are not generated by the previous executable, the instances can be easily built using the information inside the JSON files.

### Solving KP instances with DIGNEA

Since all solvers and optimisation problems are integrated in DIGNEA, we are also able to solve the KP instance generated by DIGNEA itself. Although any other KP instance can be solved as well, just consider the expected format. For this purpose we include the executable known as Genetic_KNP.

*Genetic_KNP* solves all the KP instances located inside a directory using a Parallel Genetic Algorithm. There is a directory with lots of KP instances to try out this executable. You can find them in dignea/data/instances/KnapsackProblem/**.

The expected parameters for *Genetic_KNP* are:  
  - Path to the instances directory
  - Path to save the results
  - Crossover rate for the GA
  - Population size

 The experiment is defined to run 10 repetitions of the GA using the crossover rate and population size given in params and 1e5 evaluations as stop criteria. As a result from its execution, *Genetic_KNP* will generate a JSON file with the solutions for each instance file. For example, if we run the file to solve an instance using a crossover rate $cr = 0.7$, and a population size $popsize = 10$ we would obtained something like the following:

*Please note that the number of repetition and the maximum number of evaluations to perform was reduced to speed-up the process*

```json
{
    "algorithm": {
        "avg_evolution": [],
        "crossover": "Uniform One Crossover",
        "crossover_rate": 0.699999988079071,
        "elapsed_time": 8.130000000000001e-05,
        "evaluator": "Parallel OMP Population Evaluator",
        "evolution": [],
        "max_evals": 100,
        "mutation": "Uniform One Mutation",
        "mutation_rate": 0.009999999776482582,
        "name": "Parallel Genetic Algorithm",
        "num_cores": 2,
        "pop_size": 10,
        "selection": "Binary Tournament Selection"
    },
    "bests": [
        307527.0,
        306691.0
    ],
    "name": "./GA_0.700000Uncorrelated_N_100_R_10000_1.kp",
    "problem": {
        "capacity": 994644,
        "low_limit": 0.0,
        "low_profit": 0,
        "low_weight": 0,
        "name": "Knapsack Problem",
        "num_cons": 1,
        "num_objs": 1,
        "num_vars": 100,
        "profits": [
            5842,
            9463,
            7298,
            (...)
        ],
        "up_limit": 1.0,
        "up_profit": 873083699,
        "up_weight": 0,
        "weights": [
            2801,
            6086,
            736,
           (...)
        ]
    },
    "reps_results": [
        {
            "0": {
                "cons": [
                    0.0
                ],
                "const_coeff": 0.0,
                "fitness": 304579.0,
                "num_cons": 1,
                "num_objs": 1,
                "num_vars": 100,
                "objs": [
                    304579.0
                ],
                "vars": [
                    0,
                    1,
                    1,
                    (...)
                ]
            },
            (...)
            "n_solutions": 10
        },
        (...)
    ]
}
```

### How to copy files from the container to your local machine

Once you run your experiments inside the Docker container, you can use the commands ```docker container ls``` and ```docker container cp``` to copy the results to your local machine. Note that it is mandatory that the container is running. Otherwise, it is not going to be possible to transfer files. Also, remember that the files generated inside a container will disappear once you stop the container.

First, you must known the ID of the container to get the files. This information is given by ```docker container ls```. For instance, runing the previous command will tell us that the ID for the container we just created is 4c8b3b99fb5a. We can also use part of it like 4c8b.

```bash
$ docker container ls
CONTAINER ID   IMAGE                    COMMAND   CREATED         STATUS         PORTS     NAMES
4c8b3b99fb5a   dignea/dignea:latest   "bash"    5 minutes ago   Up 5 minutes             bold_gates
```

Now that we know the container ID we can copy files to our local machine using ```docker container cp```. For example, let's copy the README.md inside the "dignea/" directory to our current directory in the local machine. For that, you should run the following:

```bash
myusername@mymachinename:~/current_directory$ docker cp 4c8b:/dignea/README.md .
```

Also, to copy files from your local machine TO the container you could use:

```bash
myusername@mymachinename:~/current_directory$ docker cp some_file.txt 4c8b:/some_file.txt
```

It is important to note that all these docker commands are run in the local machine, NOT inside the container. For more information about both commands you should refer to the Docker documentation website.


