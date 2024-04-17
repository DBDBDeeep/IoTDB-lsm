
#ifndef LSM_H
#define LSM_H

#include <iostream>
#include <queue>
#include "../disk/Disk.h"
#include "stdexcept"


using namespace std;


//extern vector<NormalMemtable*> immNormalMemtableList;
//extern vector<DelayMemtable*> immDelayMemtableList;



class LSM {
public:
    LSM() {
        activeNormalMemtable = new NormalMemtable();
        activeDelayMemtable = new DelayMemtable();
        disk = new Disk();
    }

    int memtableNum = 2;
    list<IMemtable*> immMemtableList;
    NormalMemtable* activeNormalMemtable;
    DelayMemtable* activeDelayMemtable;
    Disk* disk;

    bool isDelayData(uint64_t key);
    void insert(uint64_t key, int value);
    void insertData(IMemtable& memtable, uint64_t key, int value);
    int readData(uint64_t key);
    map<uint64_t, int> range(uint64_t start, uint64_t end);
    int diskRead(uint64_t key);
    map<uint64_t, int> diskRange(uint64_t start, uint64_t end);
    IMemtable* transforActiveToImm(IMemtable* memtable); // normal? delay?
    int flush();


    //디버깅용
    void printActiveMemtable();
    void printImmMemtable();



};

#endif


