
#ifndef IOTDB_LSM_COMPACTPROCESSOR_H
#define IOTDB_LSM_COMPACTPROCESSOR_H

#include "../storage/Memtable.cpp"
#include <iostream>

using namespace std;

class CompactProcessor {
public:
    DelayMemtable* compaction(IMemtable& memtable, vector<DelayMemtable*> delayMemtables);
};


#endif //IOTDB_LSM_COMPACTPROCESSOR_H
