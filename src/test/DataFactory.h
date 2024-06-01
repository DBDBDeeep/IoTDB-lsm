
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
#include <set>



#include "../core/DBManager.h"

#define VECTOR_LOG_PROGRESS(iteration, datasetSize) \
    if ((iteration * 100 / datasetSize.size()) % 5 == 0) { \
        cout << (iteration * 100 / datasetSize.size()) << "% \n"; \
    }

#define INT_LOG_PROGRESS(iteration, Count) \
     cout << (iteration * 100 / Count) << "% \n"; \
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
    void generateDelayedDataset(string& dataSetName, int dataNum, double outOfOrderRatio);
    void delayedTest();

    void writeToInitFile(string filePath,  set<int>& dataSet, vector<std::vector<int>> segmentRandomKeys, int lineToWrite);

    //N바이트 쓸때 시간 측정
    void writeToFile(size_t bytes);
    void readFromFile(size_t bytes);
    void generateWorkloadDataset(string initDataName, string& workloadDataName, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion, list<Record>& initDataSet, list<Record>& initTxnSet);
    void writeToWorkloadFile(const std::string& filePath, std::list<Record>& dataset);

    void generateReadRangeDataset(string initDataName, string& workloadDataName, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion, list<Record>& initDataSet, list<Record>& initTxnSet);
    void transferLinesToWorkloadFile(const std::string &filePath, int linesToRead);
    void generateDelaySegments(std::vector<std::vector<int>>& outOfOrderKeysPerSegment, int dataNum, int numOfSegments, int segmentDataNum);
    void generateO3Dataset(string& dataSetName, int dataNum, double outOfOrderRatio);
    map<int, int> generateSingleDelayDataset(std::set<int>& remainingKeys,  set<int>& dataSet, int dataNum);
    void setSingleDelayOffset(int key,  set<int>& dataSet);
    void setSegmentDelayOffset(const vector<int>& segment, size_t dataSetSize);

private:
    DBManager* tree;
    int outOfOrderCount;    //o3 data 몇개인지 (o3데이터 어디에 있는지 위치 계산할 때 필요)
    uint64_t randomIndex;  //o3 data가 삽입되는 인덱스 (o3데이터 어디에 있는지 위치 계산할 때 필요)
    string filename="dump.txt";
    vector<int> sizes; // 각 segment의 크기를 저장할 벡터
    int randomReadKey; // 랜덤 읽기를 위한 키
    int iteration = 0; // 진행률 표시를 위한 변수
    unordered_map<int, Record> singleReadSet;
    unordered_map<int, Record> rangeSet;
    vector<int> singleDelayOffsets;
    vector<int> segmentDelayOffsets;
    map<int, int>  singleDelayKeys;
    map<int, int> randomSingleKeys;
    std::vector<uint64_t > segmentDelayKeys;
    int rangeSetIdx=0;
    unordered_set<int> randomIndexMap;
};

#endif //dataFactory_H
