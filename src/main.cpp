
#include "test/DataFactory.h"
#include "test/workload/Workload.h"
#include "core/DBmanager.h"
#include "test/CompactionTest.h"
#include <iostream>

using namespace std;

//DBManager* tree;

// 테스트 리스트
void 기본테스트();
void 딜레이테스트();

int main(int argc, char* argv[]){


    DataFactory factory;
    Workload workloadA;
    /**해나 시간 측정 테스트**/
    // size_t size = 4 * 1024; // 예: 4KB
    // factory.writeToFile(size);
    // factory.readFromFile(size);
    if (argc != 8) {
        cerr << "Usage: " << argv[0] << " <initDataCount> <numSegments> <outOfOrderRatio> <readProportion> <insertProportion> <singleReadProportion> <rangeReadProportion>" << endl;
        return 1;
    }

    /**out of order data 생성 및 파일 쓰기**/
    /**parameter 설정
       o3데이터 없이 생성하고 싶으면 outOfOrderRatio = 0, numSegments = 0**/
    int initDataCount = atoi(argv[1]); // 데이터셋 크기
    int numSegments = atoi(argv[2]); // out of ordner 세그먼트 개수
    double outOfOrderRatio = stod(argv[3]); // out of order 비율
    string initfilePath = "../src/test/dataset/initData.txt";




    factory.generateDelayedDataset(initDataCount,outOfOrderRatio,numSegments);
    vector<Record> initDataSet = workloadA.readFile(initfilePath);

    /**workloadA
     * Insert 100 Read 0
     * **/

    string filePath = "../src/test/dataset/workloadA.txt";
    double readProportion = stod(argv[4]); // 읽기 작업 비율
    double insertProportion = stod(argv[5]); // 삽입 작업 비율
    double singleReadProportion = stod(argv[6]); // 단일 read 작업에 대한 읽기 작업 비율
    double rangeReadProportion = stod(argv[7]); // range 읽기 작업 비율

    factory.generateWorkloadDataset(initDataSet, filePath, readProportion, insertProportion, singleReadProportion, rangeReadProportion);
    vector<Record> datasetA = workloadA.readFile(filePath);
    workloadA.executeWorkload(datasetA);
    /**workloadB
    * Insert 70 Read 30
    * **/
//    string filePathB = "../src/test/dataset/workloadB.txt";
//    double readProportion = 0.3; // 읽기 작업 비율
//    double insertProportion = 0.7; // 삽입 작업 비율
//    double singleReadProportion = 0.5; // 단일 read 작업에 대한 읽기 작업 비율
//    double rangeReadProportion = 0.5; // range 읽기 작업 비율
    /**workloadC
    * Insert 80 Read 20
    * **/
//    string filePathC = "../src/test/dataset/workloadC.txt";
//    double readProportion = 0.2; // 읽기 작업 비율
//    double insertProportion = 0.8; // 삽입 작업 비율
//    double singleReadProportion = 0.5; // 단일 read 작업에 대한 읽기 작업 비율
//    double rangeReadProportion = 0.5; // range 읽기 작업 비율



    factory.deleteAllSSTable();


//    factory.printDelayData();

//    CompactionTest compaction;
//    compaction.runCompaction();



    DBManager* tree = workloadA.getTree();
    tree->printActiveMemtable(false);
    tree->printImmMemtable();
    tree->Disk->printSSTableList();



//    size_t size = 4 * 1024; // 예: 4KB
//
//    factory.writeToFile(size);
//    factory.readFromFile(size);



    return 0;

}

