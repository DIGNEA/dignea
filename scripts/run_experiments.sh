#!/bin/bash

echo  "Running TSP Performance with target 07"
./EIGTSPExperiment 0.7 0.8 0.9 1.0 0.85 0.15
zip -j -r tsp_instances_w85_n15_target_07.zip ./*.json ./*.tsp

echo  "Running TSP Performance with target 08"
./EIGTSPExperiment 0.8 0.7 0.9 1.0 0.85 0.15
zip -j -r tsp_instances_w85_n15_target_08.zip ./*.json ./*.tsp

echo  "Running TSP Performance with target 09"
./EIGTSPExperiment 0.9 0.7 0.8 1.0 0.85 0.15
zip -j -r tsp_instances_w85_n15_target_09.zip ./*.json ./*.tsp

echo  "Running TSP Performance with target 1"
./EIGTSPExperiment 1.0 0.7 0.8 0.9 0.85 0.15
zip -j -r tsp_instances_w85_n15_target_1.zip ./*.json ./*.tsp
