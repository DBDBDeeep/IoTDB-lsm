//
// Created by 나경 on 2024/05/11.
//

#ifndef IOTDB_DBManager_WORKLOAD_H
#define IOTDB_DBManager_WORKLOAD_H

#include "../DataFactory.h"
#include "../../core/DBManager.h"
#include "../../core/DBManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <sstream>
#include <filesystem>
#include <regex>
#include <unordered_map>

#define INT_LOG_PROGRESS(iteration, count) \
    if (count > 0 && ((iteration * 100 / count) % 10 == 0) && (iteration * 100 % count == 0)) { \
        std::cout << (iteration * 100 / count) << "% \n"; \
    }

class Workload {
public:
    vector<int> op;    //INSERT: 0, READ: 1, RANGE: 2
    vector<uint64_t> workloadKey;
    vector<pair<uint64_t, uint64_t> > workloadRangeKey;
    DBManager* tree;



    Workload(){
        if (!tree) {
            tree = new DBManager();
        }
    };
    list<Record> readFileFromStart(const std::string& filePath, int linesToRead);
    list<Record> readFileWhole(const string& filePath);
    void executeInsertWorkload(list<Record>& dataset, int start, int end);
    void executeMixedWorkload(list<Record>& dataset, int start, int end);
    void executeWorkload(list<Record>& dataset, bool isMixedWorkload);
    void cleanup();
    DBManager* getTree();
    void deleteAllSSTable();
    void makeSSTable();
    void printDelayData();
    int extractHalfLinesFromFilename(const string& filePath);
    void printReadFileDataset(const std::list<Record>& readFileDataset);
    //    list<Record> readFileFromMiddle(const std::string& filePath, int linesToRead);



private:
    void readLines(ifstream& file, std::list<Record>& dataset, int linesToRead);
    int iteration=0;
};
#endif //IOTDB_LSM_WORKLOAD_H
