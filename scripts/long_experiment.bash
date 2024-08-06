#!/bin/bash

trials=5

rm data/results.csv
echo "n,algorithm,duration_s,correct" > data/results.csv

cd build
for i in $(seq 1 $trials)
do
    ./matrix-run 0 1024 8
done

for i in $(seq 1 $trials)
do
    ./matrix-run 0 8192 1024
done

cd ..

python3 scripts/generate_graph.py "data/results.csv"
python3 scripts/mem_usage.py $max_n $step
