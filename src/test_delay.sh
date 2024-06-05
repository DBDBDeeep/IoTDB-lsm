#!/bin/bash

PROGRAM="./testIoTDB"  # 실행 파일 경로를 지정하세요

size=100000000

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <value>"
  echo "Allowed values: 0.1, 0.2, 0.3"
  exit 1
fi

VALUE=$1

# data와 files 배열 설정
case $VALUE in
  
  0.1)
    data=("data_c100000000_d0.10")
    files=("workloadA_data_c100000000_d0.10" "workloadB_data_c100000000_d0.10"  "workloadD_data_c100000000_d0.10" "workloadE_data_c100000000_d0.10" "workloadF_data_c100000000_d0.10" "workloadG_data_c100000000_d0.10")
    ;;
  0.2)
    data=("data_c100000000_d0.20")
    files=("workloadA_data_c100000000_d0.20" "workloadB_data_c100000000_d0.20" "workloadC_data_c100000000_d0.20" "workloadD_data_c100000000_d0.20" "workloadE_data_c100000000_d0.20" "workloadF_data_c100000000_d0.20" "workloadG_data_c100000000_d0.20")
    ;;
  0.3)
    data=("data_c100000000_d0.30")
    files=("workloadA_data_c100000000_d0.30" "workloadB_data_c100000000_d0.30" "workloadC_data_c100000000_d0.30" "workloadD_data_c100000000_d0.30" "workloadE_data_c100000000_d0.30" "workloadF_data_c100000000_d0.30" "workloadG_data_c100000000_d0.30")
    ;;
  *)
    echo "Invalid value: $VALUE"
    echo "Allowed values: 0.05, 0.1, 0.2, 0.3"
    exit 1
    ;;
esac

# 배열 내용 출력
for file in "${files[@]}"; do
    echo "[ IoRDB ] Running with datas=${data} and workload=${file}"
    $PROGRAM $size $data $file
done
