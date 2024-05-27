
#ifndef dataFactory_H
#define dataFactory_H

#include <random>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <cstring>


#include "../core/DBManager.h"

#define VECTOR_LOG_PROGRESS(iteration, datasetSize) \
    cout << (iteration * 100 / datasetSize.size()) << "% \n";

#define INT_LOG_PROGRESS(iteration, Count) \
    cout << (iteration * 100 / Count) << "% \n";
using namespace std;
struct Record {
    string op;
    uint64_t key;
    uint64_t start_key;
    uint64_t end_key;
};
class DataFactory {

public:
    void generateNormalDataset(int n);
    void NormalTest();
    void generateDelayedDataset(string& dataSetName, int n, double outOfOrderRatio);
    void delayedTest();
    void printDelayData();

    void writeToInitFile(string filePath, vector<uint64_t>& dataset);

    //N바이트 쓸때 시간 측정

    void writeToFile(size_t bytes);
    void readFromFile(size_t bytes);
    void generateWorkloadDataset(vector<Record>& initDataSet, string& workloadDataName, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion);
    void writeToWorkloadFile(string filePath, vector<Record>& dataset);
private:
    DBManager* tree;
    int outOfOrderCount;    //o3 data 몇개인지 (o3데이터 어디에 있는지 위치 계산할 때 필요)
    uint64_t randomIndex;  //o3 data가 삽입되는 인덱스 (o3데이터 어디에 있는지 위치 계산할 때 필요)
    string filename="dump.txt";
    vector<int> sizes; // 각 segment의 크기를 저장할 벡터
};

#endif //dataFactory_H
