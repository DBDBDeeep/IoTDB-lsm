//
// Created by 이해나 on 5/25/24.
//
#include "test/DataFactory.h"
#include "test/workload/Workload.h"

int main(int argc, char* argv[]) {

    Workload workloadA;
    DataFactory factory;

    std::string initDataName = argv[1]; //workload를 생성할 초기 dataset 파일명
    std::string workloadDataName = argv[2]; //생성할 workload 이름
    double readProportion = stod(argv[3]); // 읽기 작업 비율
    double insertProportion = stod(argv[4]); // 삽입 작업 비율
    double singleReadProportion = stod(argv[5]); // 단일 read 작업에 대한 비율
    double rangeReadProportion = stod(argv[6]); // range 읽기 작업 비율

    string initfilePath = "./test/dataset/" + initDataName+".txt";
    int halfLines = workloadA.extractHalfLinesFromFilename(initfilePath);

    //initFile 1~halfLines까지 읽어서 initDataSet에 저장
    std::list<Record> initDataSet = workloadA.readFileFromStart(initfilePath, halfLines);

    //initDataSet을 이용해서 readRangeDataset 워크로드 생성
    factory.generateReadRangeDataset(readProportion, insertProportion, singleReadProportion, rangeReadProportion, initDataSet);

    //생성된 워크로드 파일에 쓰기
    factory.transferLinesToWorkloadFile(initfilePath, workloadDataName, halfLines,readProportion,insertProportion,singleReadProportion);
}