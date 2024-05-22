//
// Created by 나경 on 2024/05/11.
//

#ifndef IOTDB_LSM_WORKLOAD_H
#define IOTDB_LSM_WORKLOAD_H

#include "../DataFactory.h"
#include "../../core/LSM.h"
#include "../../core/LSM.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <sstream>

class Workload {
private:
    vector<int> op;    //INSERT: 0, READ: 1, RANGE: 2
    vector<uint64_t> workloadKey;
    vector<pair<uint64_t, uint64_t> > workloadRangeKey;
    vector<Record> dataset;
    LSM* tree;


public:
    Workload(){
        if (!tree) {
            tree = new LSM();
        }
    };
    vector<Record> readFile(const string& filePath);
    void executeWorkload(vector<Record>& dataset);
    void cleanup();
    LSM* getTree();

};
#endif //IOTDB_LSM_WORKLOAD_H

