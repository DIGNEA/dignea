cmake_minimum_required(VERSION 3.16.3)
project(dignea LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "Diverse Instance Generator with Novelty and EAs Framework")

set(CMAKE_CXX_STANDARD 20)
enable_language(C)
add_definitions(-DOS=LINUX)

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif()

# Path configuration
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

include(FetchContent)
include(GNUInstallDirs)

# Libraries for compiling dignea
find_package(OpenMP REQUIRED)

# find_package(MPI REQUIRED)
# include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# JSON for the file generation
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1)
FetchContent_GetProperties(json)

if(NOT json_POPULATED)
        FetchContent_Populate(json)
        add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.3.2)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(NumCpp
        GIT_REPOSITORY https://github.com/dpilger26/NumCpp
        GIT_TAG Version_2.10.1)
FetchContent_MakeAvailable(NumCpp)

add_subdirectory(lib/fmt)

# Always run with the optimisation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -isystem -fPIC -fpermissive -lstdc++fs -O3 -ffast-math -Wall -march=native -mtune=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fprofile-arcs -ftest-coverage --coverage")

# Ignore Pisinger warnings
set_source_files_properties(src/dignea/algorithms/kp_heuristics/Combo.cpp
        src/dignea/algorithms/kp_heuristics/ExpKnap.cpp
        src/dignea/algorithms/kp_heuristics/MinKnap.cpp
        PROPERTIES
        COMPILE_FLAGS "-w")

# Source code
set(SOURCE_FILES
        src/dignea/experiment/AnalysisData.cpp
        src/dignea/problems/KP.cpp

        src/dignea/problems/KPNR.cpp
        src/dignea/problems/SampleProblem.cpp
        src/dignea/problems/Sphere.cpp
        src/dignea/problems/DoubleSphere.cpp
        src/dignea/problems/ZDT.cpp

        src/dignea/utilities/random/ParallelPRNG.cpp
        src/dignea/utilities/random/PseudoRandom.cpp
        src/dignea/utilities/random/RandomGenerator.cpp
        include/dignea/algorithms/singleobjective/FIGA.h
        include/dignea/algorithms/singleobjective/GenerationalGA.h
        include/dignea/algorithms/singleobjective/ParallelGeneticAlgorithm.h
        include/dignea/algorithms/singleobjective/SteadyGA.h
        include/dignea/core/AbstractEA.h
        include/dignea/core/AbstractGA.h
        include/dignea/core/Crossover.h
        include/dignea/core/Mutation.h
        include/dignea/core/Selection.h
        include/dignea/core/Problem.h
        include/dignea/core/Solution.h
        include/dignea/types/SolutionTypes.h
        include/dignea/crossovers/UniformCrossover.h
        include/dignea/crossovers/OrderCrossover.h
        include/dignea/evaluators/ParallelOMPEvaluator.h
        include/dignea/evaluators/ParallelSTL.h
        include/dignea/evaluators/PopulationEvaluator.h
        include/dignea/evolution/Evolution.h
        include/dignea/evolution/AvgEvolution.h
        include/dignea/experiment/AnalysisData.h
        include/dignea/experiment/ExperimentAnalysis.h
        include/dignea/builders/ExpBuilder.h
        include/dignea/experiment/Experiment.h
        include/dignea/mutations/UniformAllMutation.h
        include/dignea/mutations/UniformOneMutation.h
        include/dignea/mutations/SwapMutation.h
        include/dignea/mutations/ScrambleMutation.h
        include/dignea/problems/KP.h
        include/dignea/problems/KPNR.h
        include/dignea/problems/SampleProblem.h
        include/dignea/problems/Sphere.h
        include/dignea/problems/DoubleSphere.h
        include/dignea/problems/ZDT.h

        include/dignea/selections/BinaryTournamentSelection.h
        include/dignea/utilities/Comparator.h
        include/dignea/utilities/exceptions/EmptyPopulation.h
        include/dignea/utilities/exceptions/Mismatch.h
        include/dignea/utilities/exceptions/NoMOAllowed.h
        include/dignea/utilities/exceptions/NotImplemented.h
        include/dignea/utilities/exceptions/OutOfRange.h
        include/dignea/utilities/printer/JSONPrinter.h
        src/dignea/utilities/printer/JSONPrinter.cpp
        include/dignea/utilities/random/ParallelPRNG.h
        include/dignea/utilities/random/PseudoRandom.h
        include/dignea/utilities/random/RandomGenerator.h
        include/dignea/utilities/Sorter.h
        include/dignea/generator/instances/KPInstance.h
        src/dignea/generator/instances/KPInstance.cpp
        src/dignea/generator/domains/KPDomain.cpp
        include/dignea/generator/domains/KPDomain.h
        include/dignea/generator/instances/BPPInstance.h
        src/dignea/generator/instances/BPPInstance.cpp
        src/dignea/generator/domains/BPPDomain.cpp
        include/dignea/generator/domains/BPPDomain.h
        include/dignea/generator/AbstractDomain.h
        include/dignea/generator/EIG.h

        # include/dignea/generator/MOEIG.h
        include/dignea/generator/MapElites.h
        include/dignea/core/Front.h
        include/dignea/metrics/Metric.h
        include/dignea/metrics/AverageFitness.h
        include/dignea/metrics/BestFitness.h
        include/dignea/metrics/WorstFitness.h
        include/dignea/generator/evaluations/InstanceFitness.h
        include/dignea/generator/evaluations/EasyInstances.h
        include/dignea/generator/evaluations/PisPerformance.h

        include/dignea/types/GATypes.h
        include/dignea/types/CrossoverTypes.h
        include/dignea/types/MutationTypes.h
        include/dignea/types/NSTypes.h
        include/dignea/types/EvaluatorTypes.h
        include/dignea/types/SelectionTypes.h
        include/dignea/types/GATypes.h
        src/dignea/generator/evaluations/EasyInstances.cpp
        src/dignea/generator/evaluations/PisPerformance.cpp

        include/dignea/core/Replacement.h
        include/dignea/replacements/EGenerational.h
        include/dignea/replacements/ReplaceWorst.h
        include/dignea/replacements/FirstImprove.h
        include/dignea/core/Diversity.h
        include/dignea/distances/Hamming.h
        include/dignea/utilities/KNN.h
        src/dignea/utilities/KNN.cpp

        include/dignea/evolution/DiversityEvolution.h
        include/dignea/distances/Euclidean.h
        include/dignea/distances/Manhattan.h
        include/dignea/searches/NoveltySearch.h

        include/dignea/searches/Search.h
        include/dignea/generator/evaluations/Weighted.h
        include/dignea/types/ReplacementTypes.h
        include/dignea/factories/CXFactory.h
        include/dignea/factories/MutFactory.h
        include/dignea/factories/SelFactory.h
        include/dignea/factories/RepFactory.h

        include/dignea/factories/NSFactory.h
        include/dignea/builders/GABuilder.h
        include/dignea/builders/ParGABuilder.h
        include/dignea/utilities/Statistics.h
        include/dignea/builders/EIGBuilder.h
        include/dignea/problems/Rastrigin.h
        src/dignea/problems/Rastrigin.cpp
        include/dignea/algorithms/singleobjective/SimulatedAnnealing.h
        include/dignea/searches/NSFeatures.h
        include/dignea/searches/NSPerformance.h
        include/dignea/problems/TSP.h
        include/dignea/generator/domains/TSPDomain.h
        include/dignea/generator/instances/TSPInstance.h
        src/dignea/generator/domains/TSPDomain.cpp
        src/dignea/generator/instances/TSPInstance.cpp

        include/dignea/core/Heuristic.h
        include/dignea/algorithms/kp_heuristics/Default.h
        src/dignea/algorithms/kp_heuristics/Default.cpp
        include/dignea/algorithms/kp_heuristics/MPW.h
        src/dignea/algorithms/kp_heuristics/MPW.cpp

        include/dignea/algorithms/kp_heuristics/MaP.h
        src/dignea/algorithms/kp_heuristics/MaP.cpp
        include/dignea/algorithms/kp_heuristics/MiW.h
        src/dignea/algorithms/kp_heuristics/MiW.cpp

        include/dignea/algorithms/kp_heuristics/ExpKnap.h
        src/dignea/algorithms/kp_heuristics/ExpKnap.cpp
        include/dignea/algorithms/kp_heuristics/MinKnap.h
        src/dignea/algorithms/kp_heuristics/MinKnap.cpp
        include/dignea/algorithms/kp_heuristics/Combo.h
        src/dignea/algorithms/kp_heuristics/Combo.cpp
        include/dignea/algorithms/tsp_heuristics/NearestNeighbour.h
        src/dignea/algorithms/tsp_heuristics/NearestNeighbour.cpp
        include/dignea/algorithms/tsp_heuristics/Greedy.h
        src/dignea/algorithms/tsp_heuristics/Greedy.cpp

        include/dignea/algorithms/tsp_heuristics/TwoOpt.h
        src/dignea/algorithms/tsp_heuristics/TwoOpt.cpp

        include/dignea/algorithms/bpp_heuristics/NextFit.h
        src/dignea/algorithms/bpp_heuristics/NextFit.cpp

        include/dignea/algorithms/bpp_heuristics/FirstFit.h
        src/dignea/algorithms/bpp_heuristics/FirstFit.cpp

        include/dignea/algorithms/bpp_heuristics/WorstFit.h
        src/dignea/algorithms/bpp_heuristics/WorstFit.cpp

        include/dignea/algorithms/bpp_heuristics/BestFit.h
        src/dignea/algorithms/bpp_heuristics/BestFit.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src lib)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json
        OpenMP::OpenMP_CXX
        NumCpp::NumCpp
        fmt::fmt-header-only
)

if(NUMCPP_NO_USE_BOOST)
        target_compile_definitions(${PROJECT_NAME} PRIVATE -DNUMCPP_NO_USE_BOOST)
endif()

# Installation
# install(TARGETS ${PROJECT_NAME} EXPORT DigneaConfig
# RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )
# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(DIRECTORY lib/ DESTINATION ${CMAKE_INSTALL_LIBDIR})
# install(EXPORT DigneaConfig DESTINATION share/${PROJECT_NAME}/cmake)
# export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR INCLUDE_TESTS)
        enable_testing()
        add_executable(${PROJECT_NAME}Test
                test/main.cpp
                test/core/ProblemTest.cpp
                test/core/SolutionTest.cpp
                test/core/FrontTest.cpp
                test/utilities/random/random_engine/RandomGeneratorTest.cpp
                src/dignea/utilities/random/RandomGenerator.cpp
                src/dignea/utilities/random/PseudoRandom.cpp
                test/utilities/random/PRNG/PseudoRandomTest.cpp
                test/utilities/random/PRNG/ParallelPRNGTest.cpp
                test/utilities/sorting/SorterTest.cpp
                src/dignea/problems/SampleProblem.cpp
                include/dignea/problems/SampleProblem.h
                test/utilities/comparator/ComparatorTest.cpp
                include/dignea/problems/KP.h
                include/dignea/problems/BPP.h
                src/dignea/problems/BPP.cpp

                test/evaluators/PopulationEvaluatorTest.cpp
                test/evaluators/ParallelOMPEvaluatorTest.cpp
                test/algorithms/singleobjective/GenerationalGATest.cpp
                test/algorithms/singleobjective/SteadyGATest.cpp
                test/algorithms/singleobjective/FIGATest.cpp
                test/algorithms/multiobjective/NSGA2Test.cpp
                test/mutations/UniformAllMutationTest.cpp
                test/mutations/UniformOneMutationTest.cpp
                test/crossovers/UniformCrossoverTest.cpp
                test/selections/BinaryTournamentSelectionTest.cpp
                test/problems/SphereTest.cpp
                test/problems/KPTest.cpp
                test/problems/BPPTest.cpp

                include/dignea/experiment/ExperimentAnalysis.h
                src/dignea/experiment/AnalysisData.cpp
                test/algorithms/singleobjective/ParallelGeneticAlgorithmTest.cpp
                include/dignea/utilities/random/ParallelPRNG.h
                src/dignea/utilities/random/ParallelPRNG.cpp
                src/dignea/generator/domains/KPDomain.cpp
                include/dignea/generator/domains/KPDomain.h
                include/dignea/generator/AbstractDomain.h
                test/generator/instances/KPInstanceTest.cpp
                test/generator/domains/KPDomainTest.cpp
                include/dignea/generator/EIG.h
                include/dignea/generator/MapElites.h

                # include/dignea/generator/MOEIG.h
                test/problems/KPNRTest.cpp
                test/problems/SampleProblemTest.cpp
                src/dignea/utilities/printer/JSONPrinter.cpp
                test/utilities/printer/JSONPrinterTest.cpp
                test/metrics/AverageFitnessTest.cpp
                test/metrics/BestFitnessTest.cpp
                test/metrics/WorstFitnessTest.cpp
                test/experiment/ExperimentTest.cpp
                test/generator/evaluations/EasyInstancesTest.cpp
                test/generator/evaluations/WeightedTest.cpp
                test/generator/EIGTest.cpp

                # test/generator/MOEIGTest.cpp
                src/dignea/generator/evaluations/EasyInstances.cpp
                test/evolution/EvolutionTest.cpp
                test/evolution/AvgEvolutionTest.cpp
                test/experiment/AnalysisDataTest.cpp
                include/dignea/core/Diversity.h
                include/dignea/distances/Hamming.h
                test/diversity/distances/HammingTest.cpp
                include/dignea/generator/AbstractInstance.h
                test/generator/AbstractInstanceTest.cpp
                test/core/DiversityTest.cpp
                include/dignea/utilities/KNN.h
                src/dignea/utilities/KNN.cpp
                test/utilities/KNNTest.cpp
                include/dignea/evolution/DiversityEvolution.h
                test/evolution/DiversityEvolutionTest.cpp
                include/dignea/distances/Euclidean.h
                test/diversity/distances/EuclideanTest.cpp
                include/dignea/distances/Manhattan.h
                test/diversity/distances/ManhattanTest.cpp
                include/dignea/searches/NoveltySearch.h

                include/dignea/searches/Search.h
                include/dignea/generator/evaluations/Weighted.h
                include/dignea/factories/CXFactory.h
                test/factories/CXFactoryTest.cpp
                include/dignea/factories/MutFactory.h
                test/factories/MutFactoryTest.cpp
                include/dignea/factories/SelFactory.h
                test/factories/SelFactoryTest.cpp
                include/dignea/factories/RepFactory.h

                include/dignea/factories/NSFactory.h
                test/factories/NSFactoryTest.cpp
                include/dignea/builders/GABuilder.h
                test/builders/GABuilderTest.cpp
                include/dignea/builders/ParGABuilder.h
                include/dignea/builders/ExpBuilder.h
                test/builders/ExpBuilderTest.cpp
                include/dignea/utilities/Statistics.h
                test/utilities/StatisticsTest.cpp
                include/dignea/builders/EIGBuilder.h
                test/builders/EIGBuilderTest.cpp
                include/dignea/problems/Rastrigin.h
                src/dignea/problems/Rastrigin.cpp
                test/problems/RastriginTest.cpp
                include/dignea/algorithms/singleobjective/SimulatedAnnealing.h
                test/algorithms/singleobjective/SimulatedAnnealingTest.cpp
                include/dignea/searches/NSFeatures.h
                include/dignea/searches/NSPerformance.h
                test/replacements/ReplaceWorstTest.cpp
                test/replacements/FirstImproveTest.cpp
                test/replacements/EGenerationalTest.cpp
                test/distances/EuclideanTest.cpp
                test/distances/ManhattanTest.cpp
                test/distances/HammingTest.cpp
                test/searches/NSFeaturesTest.cpp
                src/dignea/problems/TSP.cpp
                test/problems/TSPTest.cpp
                test/mutations/SwapMutationTest.cpp

                # test/mutations/ScrambleMutationTest.cpp
                test/crossovers/OrderCrossoverTest.cpp
                include/dignea/generator/domains/TSPDomain.h
                include/dignea/generator/instances/TSPInstance.h
                src/dignea/generator/domains/TSPDomain.cpp
                src/dignea/generator/instances/TSPInstance.cpp
                test/generator/instances/TSPInstanceTest.cpp
                test/generator/domains/TSPDomainTest.cpp
                test/searches/NoveltySearchTest.cpp

                include/dignea/core/Heuristic.h
                include/dignea/algorithms/kp_heuristics/Default.h
                src/dignea/algorithms/kp_heuristics/Default.cpp
                test/algorithms/kp_heuristics/DefaultTest.cpp

                include/dignea/algorithms/kp_heuristics/MPW.h
                src/dignea/algorithms/kp_heuristics/MPW.cpp
                test/algorithms/kp_heuristics/MPWTest.cpp

                include/dignea/algorithms/kp_heuristics/MaP.h
                src/dignea/algorithms/kp_heuristics/MaP.cpp
                test/algorithms/kp_heuristics/MaPTest.cpp
                include/dignea/algorithms/kp_heuristics/MiW.h
                src/dignea/algorithms/kp_heuristics/MiW.cpp
                test/algorithms/kp_heuristics/MiWTest.cpp

                include/dignea/algorithms/tsp_heuristics/NearestNeighbour.h
                src/dignea/algorithms/tsp_heuristics/NearestNeighbour.cpp
                test/algorithms/tsp_heuristics/NearestNeighbourTest.cpp

                include/dignea/algorithms/tsp_heuristics/Greedy.h
                src/dignea/algorithms/tsp_heuristics/Greedy.cpp
                test/algorithms/tsp_heuristics/GreedyTest.cpp

                include/dignea/algorithms/tsp_heuristics/TwoOpt.h
                src/dignea/algorithms/tsp_heuristics/TwoOpt.cpp
                test/algorithms/tsp_heuristics/TwoOptTest.cpp

                include/dignea/algorithms/bpp_heuristics/NextFit.h
                src/dignea/algorithms/bpp_heuristics/NextFit.cpp
                test/algorithms/bpp_heuristics/NextFitTest.cpp

                include/dignea/algorithms/bpp_heuristics/FirstFit.h
                src/dignea/algorithms/bpp_heuristics/FirstFit.cpp
                test/algorithms/bpp_heuristics/FirstFitTest.cpp

                include/dignea/algorithms/bpp_heuristics/WorstFit.h
                src/dignea/algorithms/bpp_heuristics/WorstFit.cpp
                test/algorithms/bpp_heuristics/WorstFitTest.cpp

                include/dignea/algorithms/bpp_heuristics/BestFit.h
                src/dignea/algorithms/bpp_heuristics/BestFit.cpp
                test/algorithms/bpp_heuristics/BestFitTest.cpp

                test/generator/MapElitesTest.cpp
        )

        target_link_libraries(${PROJECT_NAME}Test
                PRIVATE
                ${PROJECT_NAME}
                Catch2::Catch2WithMain
                nlohmann_json::nlohmann_json
                OpenMP::OpenMP_CXX
                fmt::fmt-header-only
                NumCpp::NumCpp
        )
        target_include_directories(${PROJECT_NAME}Test
                PRIVATE
                include
                src)
        set_target_properties(${PROJECT_NAME}Test
                PROPERTIES
                COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20 -lstdc++fs"
                LINK_FLAGS "")

        if(NUMCPP_NO_USE_BOOST)
                target_compile_definitions(${PROJECT_NAME}Test PRIVATE -DNUMCPP_NO_USE_BOOST)
        endif()

        set(UNIT_TEST ${PROJECT_NAME}Test)
        add_custom_command(
                TARGET ${UNIT_TEST}
                COMMENT "Run tests"
                POST_BUILD
                COMMAND ${UNIT_TEST})

        # Coverage only on DEBUG mode
        if(INCLUDE_COVERAGE)
                # Coverage
                include(CodeCoverage.cmake)
                append_coverage_compiler_flags("-fprofile-generate -fprofile-arcs --coverage")
                set(COVERAGE_EXCLUDES
                        '${PROJECT_SOURCE_DIR}/examples/*'
                        '/usr/*'
                        '${PROJECT_SOURCE_DIR}/lib/*'
                        '${PROJECT_SOURCE_DIR}/test/*'
                        '${PROJECT_SOURCE_DIR}/build/_')
                setup_target_for_coverage_lcov(
                        NAME ${PROJECT_NAME}TestCoverage
                        EXECUTABLE ${UNIT_TEST}
                        BASE_DIRECTORY "${PROJECT_SOURCE_DIR}"
                )
        endif()
endif()

# #################################################################################
# Examples
# #################################################################################
if(INCLUDE_EXAMPLES)
        add_executable(Genetic_KNP
                examples/Genetic_KNP.cpp
                src/dignea/utilities/random/RandomGenerator.cpp
                src/dignea/utilities/random/PseudoRandom.cpp
                src/dignea/utilities/random/ParallelPRNG.cpp
                src/dignea/problems/KPNR.cpp
                src/dignea/problems/KP.cpp
                src/dignea/algorithms/kp_heuristics/Default.cpp
                src/dignea/algorithms/kp_heuristics/MPW.cpp
                src/dignea/algorithms/kp_heuristics/MaP.cpp
                src/dignea/algorithms/kp_heuristics/MiW.cpp

                src/dignea/algorithms/kp_heuristics/ExpKnap.cpp
                src/dignea/algorithms/kp_heuristics/MinKnap.cpp
                src/dignea/algorithms/kp_heuristics/Combo.cpp
        )
        target_include_directories(Genetic_KNP PRIVATE
                include
                src
        )
        target_link_libraries(Genetic_KNP
                ${PROJECT_NAME}
                nlohmann_json::nlohmann_json
                fmt::fmt-header-only
                OpenMP::OpenMP_CXX
        )

        add_executable(Pisinger_KP
                examples/Pisinger_KP.cpp
                src/dignea/utilities/random/RandomGenerator.cpp
                src/dignea/utilities/random/PseudoRandom.cpp
                src/dignea/utilities/random/ParallelPRNG.cpp
                src/dignea/problems/KPNR.cpp
                src/dignea/problems/KP.cpp
                src/dignea/algorithms/kp_heuristics/ExpKnap.cpp
                src/dignea/algorithms/kp_heuristics/MinKnap.cpp
                src/dignea/algorithms/kp_heuristics/Combo.cpp
        )
        target_include_directories(Pisinger_KP PRIVATE
                include
                src
        )
        target_link_libraries(Pisinger_KP
                ${PROJECT_NAME}
                nlohmann_json::nlohmann_json
                fmt::fmt-header-only
        )

        add_executable(EIGKPExperiment
                examples/EIGKPExperiment.cpp
                src/dignea/generator/evaluations/EasyInstances.cpp
                src/dignea/generator/domains/KPDomain.cpp
                src/dignea/generator/instances/KPInstance.cpp
                src/dignea/problems/KPNR.cpp
                src/dignea/problems/KP.cpp
        )
        target_include_directories(EIGKPExperiment PRIVATE
                include
                src
        )
        target_link_libraries(EIGKPExperiment
                ${PROJECT_NAME}
                nlohmann_json::nlohmann_json
                fmt::fmt-header-only
                OpenMP::OpenMP_CXX
        )

        add_executable(KP_HEURISTICS
                examples/KP_Heuristics.cpp
                src/dignea/utilities/random/RandomGenerator.cpp
                src/dignea/utilities/random/PseudoRandom.cpp
                src/dignea/utilities/random/ParallelPRNG.cpp
                src/dignea/problems/KPNR.cpp
                src/dignea/problems/KP.cpp
                include/dignea/experiment/ExperimentAnalysis.h
                src/dignea/generator/evaluations/PisPerformance.cpp
        )
        target_include_directories(KP_HEURISTICS PRIVATE
                include
                src
        )
        target_link_libraries(KP_HEURISTICS
                ${PROJECT_NAME}
                nlohmann_json::nlohmann_json
                fmt::fmt-header-only
                NumCpp::NumCpp

                # ${MPI_C_LIBRARIES}
        )

        add_executable(BPP_HEURISTICS
                examples/BPP_Heuristics.cpp
                src/dignea/problems/BPP.cpp
                src/dignea/algorithms/bpp_heuristics/NextFit.cpp
                src/dignea/algorithms/bpp_heuristics/FirstFit.cpp
                src/dignea/algorithms/bpp_heuristics/WorstFit.cpp
                src/dignea/algorithms/bpp_heuristics/BestFit.cpp
        )
        target_include_directories(BPP_HEURISTICS PRIVATE
                include
                src
        )
        target_link_libraries(BPP_HEURISTICS
                ${PROJECT_NAME}
                nlohmann_json::nlohmann_json
                fmt::fmt-header-only

                # ${MPI_C_LIBRARIES}
        )

        add_executable(MapElitesKP
                examples/MapElites_KPDomain.cpp
                src/dignea/utilities/random/RandomGenerator.cpp
                src/dignea/utilities/random/PseudoRandom.cpp
                src/dignea/utilities/random/ParallelPRNG.cpp
                src/dignea/problems/KPNR.cpp
                src/dignea/problems/KP.cpp
                include/dignea/experiment/ExperimentAnalysis.h
        )
        target_include_directories(MapElitesKP PRIVATE
                include
                src
        )
        target_link_libraries(MapElitesKP
                ${PROJECT_NAME}
                nlohmann_json::nlohmann_json
                fmt::fmt-header-only
                NumCpp::NumCpp
        )
endif()
