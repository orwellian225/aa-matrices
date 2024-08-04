#!/usr/bin/env fish

set max_n 512
set step 8
set trials 5

rm data/results.csv
echo "n,algorithm,duration_s,correct" > data/results.csv

cd build
for i in (seq 1 $trials)
    ./matrix-run 0 $max_n $step
end
cd ..
python scripts/generate_graph.py "data/results.csv"
python scripts/mem_usage.py $max_n $step
