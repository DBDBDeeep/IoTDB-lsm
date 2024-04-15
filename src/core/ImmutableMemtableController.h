#ifndef IOTDB_LSM_IMMUTABLEMEMTABLECONTROLLER_H
#define IOTDB_LSM_IMMUTABLEMEMTABLECONTROLLER_H

#include "../storage/Memtable.cpp"
#include "CompactProcessor.h"
#include <iostream>
#include <queue>

using namespace std;

extern vector<NormalMemtable*> normalMemtableList_M1;

extern vector<DelayMemtable*> delayMemtableList_M1;
extern vector<NormalMemtable*> normalMemtableList_M2;
extern vector<DelayMemtable*> delayMemtableList_M2;
extern queue<IMemtable*> compactionQueue;
extern queue<IMemtable*> flushQueue;
extern CompactProcessor* compactProcessor;

class ImmutableMemtableController {
public:
    ImmutableMemtableController(){
        compactProcessor = new CompactProcessor();
    }
    int read(unsigned int key);
    map<unsigned int, int> range(unsigned int start, unsigned int end);
    void putMemtableToQueue(IMemtable&);
private:
    void compaction();
    void transformM1toM2(IMemtable&);
};



#endif //IOTDB_LSM_IMMUTABLEMEMTABLECONTROLLER_H
