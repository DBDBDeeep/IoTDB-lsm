#!/bin/bash

EXECUTABLE="./makeWorkload"

datasets=(data_100000000_0.10 data_100000000_0.20 data_100000000_0.30)
workload="workload"
readings=(2 10 20)
insertings=(98 90 80)

length=${#readings[@]}

for dataset in "${datasets[@]}"; do
    # 두 배열을 동시에 순회
    for (( i=0; i<$length; i++ ))
    do
        echo "Workload with dataset=${dataset} and Reading: ${readings[$i]}, Inserting: ${insertings[$i]}, V1"
        $EXECUTABLE $dataset $workload ${readings[$i]} ${insertings[$i]} 0.5 0.5

        echo "Workload with dataset=${dataset} and Reading: ${readings[$i]}, Inserting: ${insertings[$i]}, V2"
        $EXECUTABLE $dataset $workload ${readings[$i]} ${insertings[$i]} 0.2 0.8
    done
done

echo "make workload completed."
