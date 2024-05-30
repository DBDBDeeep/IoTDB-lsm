
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
    if ((iteration * 100 / datasetSize.size()) % 5 == 0) { \
        cout << (iteration * 100 / datasetSize.size()) << "% \n"; \
    }

#define INT_LOG_PROGRESS(iteration, Count) \
    if ((iteration * 100 / Count) % 5 == 0) { \
        cout << (iteration * 100 / Count) << "% \n"; \
    }
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

    void writeToInitFile(string filePath, vector<uint64_t>& dataset, unordered_multimap<int, uint64_t>& singleRandomKeys, unordered_set<uint64_t>& segmentRandomKeys, int lineToWrite);

    //N바이트 쓸때 시간 측정
    void writeToFile(size_t bytes);
    void readFromFile(size_t bytes);
    void generateWorkloadDataset(string initDataName, string& workloadDataName, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion, list<Record>& initDataSet, list<Record>& initTxnSet);
    void writeToWorkloadFile(const std::string& filePath, std::list<Record>& dataset);

    int setSegmentDelayOffset(const vector<int>& segment, size_t dataSetSize);
    void generateReadRangeDataset(string initDataName, string& workloadDataName, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion, list<Record>& initDataSet, list<Record>& initTxnSet);
    void transferLinesToWorkloadFile(const std::string &filePath, int linesToRead);
    void generateDelaySegments(std::unordered_multimap<int, vector<int>>& outOfOrderKeysPerSegment, int dataNum, int numOfSegments);
    void generateO3Dataset(string& dataSetName, int dataNum, double outOfOrderRatio);
    unordered_multimap<int, uint64_t> generateSingleDelayDataset(unordered_set<uint64_t>& remainingKeys, vector<uint64_t>& dataSet);
    int setSingleDelayOffset(int key, vector<uint64_t>& dataSet);

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
    int rangeSetIdx=0;
};

#endif //dataFactory_H
