#ifndef MockDisk_H
#define MockDisk_H

#include <list>
#include <limits>
#include <algorithm>
#include "SSTable.h" //
#include "../core/memtable/IMemtable.h"


struct MockDisk {
private:
    int compactionCount;
    int flushCount;

public:
    list<SSTable*> normalSSTables;
    list<SSTable*> delaySSTables;
    int readCount;

    bool compaction();
    int read(uint64_t key);
    map<uint64_t, int> range(uint64_t start, uint64_t end);
    bool flush(IMemtable* mem, Type t);
    void printSSTableList();
};

#endif // MockDisk_H
