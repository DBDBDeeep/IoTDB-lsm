#ifndef IOTDB_LSM_LSM_H
#define IOTDB_LSM_LSM_H

#include <iostream>
#include "Memtable.cpp"
#include "Disk.cpp"

using namespace std;

const size_t DELAY_SIZE = 40;
const size_t NORMAL_SIZE = 80;
extern NormalMemtable* activeNormalMemtable;
extern DelayMemtable* activeDelayMemtable;
extern NormalMemtable* immNormalMemtable;
extern DelayMemtable* activeDelayMemtable;
extern Disk* disk;

class LSM {
public:
    LSM() {
        activeNormalMemtable = new NormalMemtable();
        activeDelayMemtable = new DelayMemtable();
    }

    bool isFull(Memtable& memtable);
    void insert(unsigned int key, int value);
    void insertData(Memtable& memtable, unsigned int key, int value);
    int readData(unsigned int key);
    int diskRead(unsigned int key);
    int diskRange(unsigned int start, unsigned int end);
    map<unsigned int, int> range(unsigned int start, unsigned int end);
    bool convertActiveToImm(Memtable& memtable); // normal? delay?
    bool createActiveNormalMemtable();
    bool createActiveDelayMemtable();
};


#endif //IOTDB_LSM_LSM_H
