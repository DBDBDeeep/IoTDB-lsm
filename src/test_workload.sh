#!/bin/bash

PROGRAM="./testIoTDB"  # 실행 파일 경로를 지정하세요

size=100000000
datas=("data_c100000000_d0.10" "data_c100000000_d0.20" "data_c100000000_d0.30")

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <value>"
  echo "Allowed values: A B D"
  exit 1
fi

VALUE=$1

# data와 files 배열 설정
case $VALUE in
  A)
    files=("workloadA_data_c100000000_d0.10" "workloadA_data_c100000000_d0.20" "workloadA_data_c100000000_d0.30")
    ;;
  B)
    files=("workloadB_data_c100000000_d0.10" "workloadB_data_c100000000_d0.20" "workloadB_data_c100000000_d0.30")
    ;;

  D)
    files=("workloadC_data_c100000000_d0.10" "workloadD_data_c100000000_d0.20" "workloadD_data_c100000000_d0.30")
    ;;
  
  *)
    echo "Invalid value: $VALUE"
    echo "Allowed values: A, B, C, D, E, F, G"
    exit 1
    ;;
esac

# datas와 files에서 하나씩 선택하여 실행
for i in "${!datas[@]}"; do
    data=${datas[$i]}
    file=${files[$i]}
    echo "[ IoRDB ] Running with data=${data} and workload=${file}"
    $PROGRAM $size $data $file
done
