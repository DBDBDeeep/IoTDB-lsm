//
// Created by 이해나 on 5/25/24.
//
#include "test/DataFactory.h"
#include "test/workload/Workload.h"
#include "test/CompactionTest.h"

int main(int argc, char* argv[]) {
    Workload workloadA;
    string filePath = "../src/test/dataset/workloadA.txt";

    vector<Record> datasetA = workloadA.readFile(filePath);
    workloadA.executeWorkload(datasetA);
    workloadA.makeSSTable();


    CompactionTest compacton;
    compacton.runCompaction();




}