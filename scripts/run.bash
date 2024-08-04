#!/bin/bash

max_n=$1
step=$2
trials=$3

rm data/results.csv
echo "n,algorithm,duration_s,correct" > data/results.csv

cd build
for i in {1..$trials}
do
    ./matrix-run 0 $max_n $step
done
cd ..

python3 scripts/generate_graph.py "data/results.csv"
python3 scripts/mem_usage.py $max_n $step
