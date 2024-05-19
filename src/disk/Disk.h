#ifndef DISK_H
#define DISK_H

#include <list>
#include "SSTable.h" //
#include "../core/memtable/IMemtable.h"


struct Disk {
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
    bool flush(IMemtable* mem);
    void printSSTableList();
};

#endif // DISK_H
