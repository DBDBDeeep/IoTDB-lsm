
#ifndef dataFactory_H
#define dataFactory_H

#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_set>

#include "../core/LSM.h"

#define LOG_PROGRESS(iteration, datasetSize) \
    cout << (iteration * 100 / datasetSize.size()) << "% \n";

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
    void generateDelayedDataset(int n, double outOfOrderRatio, int numSegments);
    void delayedTest();
    void printDelayData();
    void writeToInitFile(string filePath, vector<uint64_t>& dataset);
    void writeToFile(size_t bytes);
    void readFromFile(size_t bytes);
    void generateWorkloadDataset(vector<Record>& initDataSet, string filePath, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion);
    void writeToWorkloadFile(string filePath, vector<Record>& dataset);
private:
    LSM* tree;
    uint64_t randomIndex;  //o3 data가 삽입되는 인덱스 (o3데이터 어디에 있는지 위치 계산할 때 필요)
    string filename="dump.txt";
    vector<int> sizes; // 각 segment의 크기를 저장할 벡터
};

#endif //dataFactory_H
