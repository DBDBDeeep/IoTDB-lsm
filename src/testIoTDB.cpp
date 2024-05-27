//
// Created by 이해나 on 5/25/24.
//
#include "test/DataFactory.h"
#include "test/workload/Workload.h"
#include "test/CompactionTest.h"

int main(int argc, char* argv[]) {
    Workload workloadA;
    string initDataNum = argv[1]; //ex) "10000";
    string workloadFileName = argv[2]; //ex) "workloadA_r0.3_i0.7_V2";
    string filePath = "../src/test/dataset/" + workloadFileName;
    vector<Record> datasetA = workloadA.readFile(filePath);
    workloadA.executeWorkload(datasetA, stoi(initDataNum), workloadFileName);
    workloadA.makeSSTable();


    CompactionTest compacton;
    compacton.runCompaction();




    workloadA.makeSSTable();
    
    CompactionTest compacton;
    compacton.runCompaction();


}