//
// Created by 이해나 on 5/25/24.
//
#include "test/DataFactory.h"
#include "test/workload/Workload.h"

int main(int argc, char* argv[]) {

    Workload workloadA;
    DataFactory factory;
    string initfilePath = "../src/test/dataset/initData.txt";

    vector<Record> initDataSet = workloadA.readFile(initfilePath);
    double readProportion = stod(argv[1]); // 읽기 작업 비율
    double insertProportion = stod(argv[2]); // 삽입 작업 비율
    double singleReadProportion = stod(argv[3]); // 단일 read 작업에 대한 비율
    double rangeReadProportion = stod(argv[4]); // range 읽기 작업 비율

    string filePath = "../src/test/dataset/workloadA.txt";
    factory.generateWorkloadDataset(initDataSet, filePath, readProportion, insertProportion, singleReadProportion, rangeReadProportion);
}