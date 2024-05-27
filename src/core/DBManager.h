
#ifndef DBManager_H
#define DBManager_H

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include "disk/MockDisk.h"
#include "stdexcept"


using namespace std;


//extern vector<NormalMemtable*> immNormalMemtableList;
//extern vector<DelayMemtable*> immDelayMemtableList;



class DBManager {
public:
    DBManager() {
        activeNormalMemtable = new NormalMemtable(++currentId);
        activeDelayMemtable = new DelayMemtable(++currentId);
        Disk = new MockDisk();
    }
    int currentId = 0;
    int memtableNum = 4;
    list<IMemtable*> immMemtableList;
    NormalMemtable* activeNormalMemtable;
    DelayMemtable* activeDelayMemtable;
    MockDisk* Disk;
    int fileCounter = 0;  //testㅑㅜ

    bool isDelayData(uint64_t key);
    void insert(uint64_t key, int value);
    void insertData(IMemtable& memtable, uint64_t key, int value);
    int readData(uint64_t key);
    map<uint64_t, int> range(uint64_t start, uint64_t end);
    int DiskRead(uint64_t key);
    map<uint64_t, int> DiskRange(uint64_t start, uint64_t end);
    IMemtable* transformActiveToImm(IMemtable* memtable); // normal? delay?
    int flush();


    //디버깅용
    void printActiveMemtable(bool printKV);
    void printImmMemtable();
    void makeFile(const vector<pair<uint64_t, int>>& sortedData,int flag);

};

#endif


