//
// Created by 이해나 on 5/25/24.
//
#include "test/DataFactory.h"
#include "test/workload/Workload.h"

int main(int argc, char* argv[]) {
    Workload workloadA;
    string filePath = argv[1]; //ex) "../src/test/dataset/R0.3_I0.7_V2.txt";

    vector<Record> datasetA = workloadA.readFile(filePath);
    workloadA.executeWorkload(datasetA);

}