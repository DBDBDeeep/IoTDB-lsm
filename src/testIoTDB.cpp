//
// Created by 이해나 on 5/25/24.
//
#include "test/DataFactory.h"
#include "test/workload/Workload.h"
#include "test/CompactionTest.h"

int main(int argc, char* argv[]) {
    Workload workloadA;
    string initDataNum = argv[1]; //ex) "10000";
    string initFileName = argv[2]; //ex) "dataB_c20000000_d0.2";
    string workloadFileName = argv[3]; //ex) "workloadA_r0.3_i0.7_V2";

    string initFilePath = "../src/test/dataset/" + initFileName+".txt";
    string workloadFilePath = "../src/test/dataset/workload/" + workloadFileName+".txt";
    int halfLines = workloadA.extractHalfLinesFromFilename(initFilePath);

    /**Init Dataset의 50%는 워크로드 수행을 위해 존재하던 데이터로 가정하고 INSERT작업 선행할 데이터셋 저장*/
    std::list<Record> initWorkload = workloadA.readFileFromStart(initFilePath, halfLines);
    /**실질 INSERT, READ, RANGE 워크로드 데이터셋 저장*/
    std::list<Record> mixedWorkload = workloadA.readFileWhole(workloadFilePath);

    workloadA.executeWorkload(initWorkload, false);
    workloadA.executeWorkload(mixedWorkload, true);

    cout<<"\ndisk read 횟수 : "<<workloadA.tree->Disk->readCount<<"\n";
    
    workloadA.printDelayData();

    workloadA.deleteAllSSTable();

    workloadA.makeSSTable();

    CompactionTest compacton;
    compacton.runCompaction();


}