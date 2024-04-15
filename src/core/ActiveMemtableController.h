#ifndef IOTDB_LSM_ACTIVEMEMTABLECONTROLLER_H
#define IOTDB_LSM_ACTIVEMEMTABLECONTROLLER_H

#include "../storage/Memtable.cpp"
using namespace std;

extern vector<NormalMemtable*> activeNormalMemtableList;
extern vector<DelayMemtable*> activeDelayMemtableList;

class ActiveMemtableController {
public:
    void insert(unsigned int key, int value);
    bool isDelayData(unsigned int key);
};


#endif //IOTDB_LSM_ACTIVEMEMTABLECONTROLLER_H
