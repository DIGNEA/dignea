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
      [ "Build DIGNEA", "index.html#autotoc_md0", [
        [ "Docker Image", "index.html#autotoc_md1", null ],
        [ "Dependencies", "index.html#autotoc_md2", null ]
      ] ],
      [ "Publications", "index.html#autotoc_md3", null ],
      [ "How DIGNEA works.", "md_docs_markdown_00_getting_started.html#autotoc_md5", [
        [ "Main components.", "md_docs_markdown_00_getting_started.html#autotoc_md6", null ],
        [ "Results", "md_docs_markdown_00_getting_started.html#autotoc_md7", null ],
        [ "More information", "md_docs_markdown_00_getting_started.html#autotoc_md8", null ]
      ] ]
    ] ],
    [ "Instance Generation Example", "md_docs_markdown_01_0_mea_example.html", [
      [ "Generating Knapsack Problem Instances with DIGNEA", "md_docs_markdown_01_0_mea_example.html#autotoc_md10", [
        [ "Parameter configuration", "md_docs_markdown_01_0_mea_example.html#autotoc_md11", [
          [ "EIG configuration", "md_docs_markdown_01_0_mea_example.html#autotoc_md12", null ],
          [ "Knapsack Problem instances", "md_docs_markdown_01_0_mea_example.html#autotoc_md13", null ]
        ] ],
        [ "Creating a portfolio of solvers", "md_docs_markdown_01_0_mea_example.html#autotoc_md14", null ],
        [ "Instantiate a EIG object", "md_docs_markdown_01_0_mea_example.html#autotoc_md15", null ],
        [ "Run the experiment", "md_docs_markdown_01_0_mea_example.html#autotoc_md16", null ],
        [ "Example of results", "md_docs_markdown_01_0_mea_example.html#autotoc_md17", null ]
      ] ]
    ] ],
    [ "Create an algorithm", "md_docs_markdown_01_create_algorithm.html", [
      [ "Creating the base Simulated Annealing class.", "md_docs_markdown_01_create_algorithm.html#autotoc_md19", null ],
      [ "Define the methods in SimulatedAnnealing", "md_docs_markdown_01_create_algorithm.html#autotoc_md20", null ],
      [ "Define the remaining methods.", "md_docs_markdown_01_create_algorithm.html#autotoc_md21", [
        [ "Constructor", "md_docs_markdown_01_create_algorithm.html#autotoc_md22", null ],
        [ "Run", "md_docs_markdown_01_create_algorithm.html#autotoc_md23", null ],
        [ "Perturbation", "md_docs_markdown_01_create_algorithm.html#autotoc_md24", null ],
        [ "Population management", "md_docs_markdown_01_create_algorithm.html#autotoc_md25", null ],
        [ "Progress", "md_docs_markdown_01_create_algorithm.html#autotoc_md26", null ],
        [ "Getters and Setters", "md_docs_markdown_01_create_algorithm.html#autotoc_md27", null ]
      ] ]
    ] ],
    [ "Create a problem", "md_docs_markdown_02_create_problem.html", [
      [ "Creating the Travelling Salesman Problem (TSP)", "md_docs_markdown_02_create_problem.html#autotoc_md29", [
        [ "Defining the methods and attributes for TSP", "md_docs_markdown_02_create_problem.html#autotoc_md30", null ],
        [ "TSP implementation in TSP.cpp", "md_docs_markdown_02_create_problem.html#autotoc_md31", [
          [ "Constructors", "md_docs_markdown_02_create_problem.html#autotoc_md32", null ],
          [ "Auxiliary building methods", "md_docs_markdown_02_create_problem.html#autotoc_md33", null ],
          [ "Create new random solutions", "md_docs_markdown_02_create_problem.html#autotoc_md34", null ],
          [ "Evaluation", "md_docs_markdown_02_create_problem.html#autotoc_md35", null ],
          [ "Getters", "md_docs_markdown_02_create_problem.html#autotoc_md36", null ],
          [ "JSON", "md_docs_markdown_02_create_problem.html#autotoc_md37", null ]
        ] ]
      ] ]
    ] ],
    [ "Create a instance generation problem", "md_docs_markdown_03_create_instance_generator.html", [
      [ "Creating the Travelling Salesman Problem (TSP)", "md_docs_markdown_03_create_instance_generator.html#autotoc_md39", null ],
      [ "Create the new AbstractDomain subclass: TSPDomain class.", "md_docs_markdown_03_create_instance_generator.html#autotoc_md40", [
        [ "Constructors", "md_docs_markdown_03_create_instance_generator.html#autotoc_md41", null ],
        [ "Generate Optimization Problem from Instance Solution", "md_docs_markdown_03_create_instance_generator.html#autotoc_md42", null ],
        [ "Create new solutions", "md_docs_markdown_03_create_instance_generator.html#autotoc_md43", null ],
        [ "Evaluation and feature calculation", "md_docs_markdown_03_create_instance_generator.html#autotoc_md44", null ],
        [ "Getters and Setters", "md_docs_markdown_03_create_instance_generator.html#autotoc_md45", null ]
      ] ],
      [ "Create the solution for the instance generation problem: TSPInstance class.", "md_docs_markdown_03_create_instance_generator.html#autotoc_md46", [
        [ "Constructors", "md_docs_markdown_03_create_instance_generator.html#autotoc_md47", null ],
        [ "Representation", "md_docs_markdown_03_create_instance_generator.html#autotoc_md48", null ]
      ] ]
    ] ],
    [ "DIGNEA through Docker containers", "md_docs_markdown_04_docker_usage.html", [
      [ "How to download DIGNEA from Docker Hub", "md_docs_markdown_04_docker_usage.html#autotoc_md50", null ],
      [ "How to run a container with DIGNEA", "md_docs_markdown_04_docker_usage.html#autotoc_md51", null ],
      [ "How to build DIGNEA", "md_docs_markdown_04_docker_usage.html#autotoc_md52", null ],
      [ "Examples and tests", "md_docs_markdown_04_docker_usage.html#autotoc_md53", [
        [ "Test suite", "md_docs_markdown_04_docker_usage.html#autotoc_md54", null ],
        [ "Generating KP instances", "md_docs_markdown_04_docker_usage.html#autotoc_md55", [
          [ "KP_HEURISTICS", "md_docs_markdown_04_docker_usage.html#autotoc_md56", null ],
          [ "EIGKPExperiment:", "md_docs_markdown_04_docker_usage.html#autotoc_md57", null ]
        ] ],
        [ "Solving KP instances with DIGNEA", "md_docs_markdown_04_docker_usage.html#autotoc_md58", null ],
        [ "How to copy files from the container to your local machine", "md_docs_markdown_04_docker_usage.html#autotoc_md59", null ]
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
"classAbstractSolver.html#a4c82c2a850619052450a99756230c09e",
"classInstanceFitness.html#a6bc452939acfc2daceb99b59cda60767",
"classProblem.html#a6e6835693ddc884b24d27873f328cd67",
"dir_794c03e1b5451c3e8bad869a9e4f9e58.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';