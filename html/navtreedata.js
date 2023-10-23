/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "dignea", "index.html", [
    [ "Getting Started", "md_docs_markdown_00_getting_started.html", [
      [ "Build DIGNEA", "index.html#autotoc_md6", [
        [ "Docker Image", "index.html#autotoc_md7", null ],
        [ "Dependencies", "index.html#autotoc_md8", null ]
      ] ],
      [ "Publications", "index.html#autotoc_md9", null ],
      [ "How DIGNEA works.", "md_docs_markdown_00_getting_started.html#autotoc_md11", [
        [ "Main components.", "md_docs_markdown_00_getting_started.html#autotoc_md12", null ],
        [ "Results", "md_docs_markdown_00_getting_started.html#autotoc_md13", null ],
        [ "More information", "md_docs_markdown_00_getting_started.html#autotoc_md14", null ]
      ] ]
    ] ],
    [ "Instance Generation Example", "md_docs_markdown_01_0_mea_example.html", [
      [ "Generating Knapsack Problem Instances with DIGNEA", "md_docs_markdown_01_0_mea_example.html#autotoc_md16", [
        [ "Parameter configuration", "md_docs_markdown_01_0_mea_example.html#autotoc_md17", [
          [ "EIG configuration", "md_docs_markdown_01_0_mea_example.html#autotoc_md18", null ],
          [ "Knapsack Problem instances", "md_docs_markdown_01_0_mea_example.html#autotoc_md19", null ]
        ] ],
        [ "Creating a portfolio of solvers", "md_docs_markdown_01_0_mea_example.html#autotoc_md20", null ],
        [ "Instantiate a EIG object", "md_docs_markdown_01_0_mea_example.html#autotoc_md21", null ],
        [ "Run the experiment", "md_docs_markdown_01_0_mea_example.html#autotoc_md22", null ],
        [ "Example of results", "md_docs_markdown_01_0_mea_example.html#autotoc_md23", null ]
      ] ]
    ] ],
    [ "Create an algorithm", "md_docs_markdown_01_create_algorithm.html", [
      [ "Creating the base Simulated Annealing class.", "md_docs_markdown_01_create_algorithm.html#autotoc_md25", null ],
      [ "Define the methods in SimulatedAnnealing", "md_docs_markdown_01_create_algorithm.html#autotoc_md26", null ],
      [ "Define the remaining methods.", "md_docs_markdown_01_create_algorithm.html#autotoc_md27", [
        [ "Constructor", "md_docs_markdown_01_create_algorithm.html#autotoc_md28", null ],
        [ "Run", "md_docs_markdown_01_create_algorithm.html#autotoc_md29", null ],
        [ "Perturbation", "md_docs_markdown_01_create_algorithm.html#autotoc_md30", null ],
        [ "Population management", "md_docs_markdown_01_create_algorithm.html#autotoc_md31", null ],
        [ "Progress", "md_docs_markdown_01_create_algorithm.html#autotoc_md32", null ],
        [ "Getters and Setters", "md_docs_markdown_01_create_algorithm.html#autotoc_md33", null ]
      ] ]
    ] ],
    [ "Create a problem", "md_docs_markdown_02_create_problem.html", [
      [ "Creating the Travelling Salesman Problem (TSP)", "md_docs_markdown_02_create_problem.html#autotoc_md35", [
        [ "Defining the methods and attributes for TSP", "md_docs_markdown_02_create_problem.html#autotoc_md36", null ],
        [ "TSP implementation in TSP.cpp", "md_docs_markdown_02_create_problem.html#autotoc_md37", [
          [ "Constructors", "md_docs_markdown_02_create_problem.html#autotoc_md38", null ],
          [ "Auxiliary building methods", "md_docs_markdown_02_create_problem.html#autotoc_md39", null ],
          [ "Create new random solutions", "md_docs_markdown_02_create_problem.html#autotoc_md40", null ],
          [ "Evaluation", "md_docs_markdown_02_create_problem.html#autotoc_md41", null ],
          [ "Getters", "md_docs_markdown_02_create_problem.html#autotoc_md42", null ],
          [ "JSON", "md_docs_markdown_02_create_problem.html#autotoc_md43", null ]
        ] ]
      ] ]
    ] ],
    [ "Create a instance generation problem", "md_docs_markdown_03_create_instance_generator.html", [
      [ "Creating the Travelling Salesman Problem (TSP)", "md_docs_markdown_03_create_instance_generator.html#autotoc_md45", null ],
      [ "Create the new AbstractDomain subclass: TSPDomain class.", "md_docs_markdown_03_create_instance_generator.html#autotoc_md46", [
        [ "Constructors", "md_docs_markdown_03_create_instance_generator.html#autotoc_md47", null ],
        [ "Generate Optimization Problem from Instance Solution", "md_docs_markdown_03_create_instance_generator.html#autotoc_md48", null ],
        [ "Create new solutions", "md_docs_markdown_03_create_instance_generator.html#autotoc_md49", null ],
        [ "Evaluation and feature calculation", "md_docs_markdown_03_create_instance_generator.html#autotoc_md50", null ],
        [ "Getters and Setters", "md_docs_markdown_03_create_instance_generator.html#autotoc_md51", null ]
      ] ],
      [ "Create the solution for the instance generation problem: TSPInstance class.", "md_docs_markdown_03_create_instance_generator.html#autotoc_md52", [
        [ "Constructors", "md_docs_markdown_03_create_instance_generator.html#autotoc_md53", null ],
        [ "Representation", "md_docs_markdown_03_create_instance_generator.html#autotoc_md54", null ]
      ] ]
    ] ],
    [ "DIGNEA through Docker containers", "md_docs_markdown_04_docker_usage.html", [
      [ "How to download DIGNEA from Docker Hub", "md_docs_markdown_04_docker_usage.html#autotoc_md56", null ],
      [ "How to run a container with DIGNEA", "md_docs_markdown_04_docker_usage.html#autotoc_md57", null ],
      [ "How to build DIGNEA", "md_docs_markdown_04_docker_usage.html#autotoc_md58", null ],
      [ "Examples and tests", "md_docs_markdown_04_docker_usage.html#autotoc_md59", [
        [ "Test suite", "md_docs_markdown_04_docker_usage.html#autotoc_md60", null ],
        [ "Generating KP instances", "md_docs_markdown_04_docker_usage.html#autotoc_md61", [
          [ "KP_HEURISTICS", "md_docs_markdown_04_docker_usage.html#autotoc_md62", null ],
          [ "EIGKPExperiment:", "md_docs_markdown_04_docker_usage.html#autotoc_md63", null ]
        ] ],
        [ "Solving KP instances with DIGNEA", "md_docs_markdown_04_docker_usage.html#autotoc_md64", null ],
        [ "How to copy files from the container to your local machine", "md_docs_markdown_04_docker_usage.html#autotoc_md65", null ]
      ] ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Related Functions", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"AbstractDomain_8h.html",
"classAbstractDomain.html#a2350e85050cfbcc81ffde1a4fd718866",
"classBestFit.html#a99a70c80bb78982c98900bc5eeddfec9",
"classHeuristic.html#a728f1d7e2224e35ec836aca88b2e26b6",
"classNoveltySearch.html#a3b8d44905ba872ca0ce8436e5f882aae",
"classTSP.html#a75c4770797dba482cc10e597425b32d2",
"md_docs_markdown_03_create_instance_generator.html#autotoc_md46"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';