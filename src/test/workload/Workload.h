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


class Workload {
private:
    vector<int> op;    //INSERT: 0, READ: 1, RANGE: 2
    vector<uint64_t> workloadKey;
    vector<pair<uint64_t, uint64_t> > workloadRangeKey;
    DBManager* tree;


public:
    Workload(){
        if (!tree) {
            tree = new DBManager();
        }
    };
    vector<Record> readFile(const string& filePath);
    void executeInsertWorkload(vector<Record>& dataset, int start, int end);
    void executeMixedWorkload(vector<Record>& dataset, int start, int end);
    void executeWorkload(vector<Record>& dataset, int initDataNum);
    void cleanup();
    DBManager* getTree();
    void deleteAllSSTable();
    void makeSSTable();
    void printDelayData();

};
#endif //IOTDB_LSM_WORKLOAD_H
