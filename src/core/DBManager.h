
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
#include "FlushController.cpp"


using namespace std;


//extern vector<NormalMemtable*> immNormalMemtableList;
//extern vector<DelayMemtable*> immDelayMemtableList;



class DBManager {
public:
    DBManager() {
        activeNormalMemtable = new NormalMemtable(++currentId);
        activeDelayMemtable = new DelayMemtable(++currentId);
        Disk = new MockDisk();
        flushController = new FlushController(Disk, flushQueue);
    }
    int currentId = 0;
    int memtableNum = 4;
    list<IMemtable*> immMemtableList;
    queue<IMemtable*> flushQueue;
    NormalMemtable* activeNormalMemtable;
    DelayMemtable* activeDelayMemtable;
    MockDisk* Disk;
    FlushController* flushController;
    int fileCounter = 0;  //test
    int diskReadCnt=0;
    uint64_t diskReadData=0;

    map<int, uint64_t> Nstart;
    map<int, uint64_t> Nlast;
    map<int, uint64_t> Dstart;
    map<int, uint64_t> Dlast;
    int Nmin=0;
    int Nmax=0;
    int Dmin=0;
    int Dmax=0;

    void deleteMem(map<int, uint64_t> imm,int id){
        imm.erase(id);

    }

    // 최소값을 반환하는 함수
uint64_t setMin(const std::map<int, uint64_t>& m) {
    if (m.empty()) {
        throw std::runtime_error("Map is empty");
    }
    int minKey = std::numeric_limits<int>::max();
    for (const auto& [key, value] : m) {
        if (key < minKey) {
            minKey = key;
        }
    }
    return minKey;
}

// 최대값을 반환하는 함수
uint64_t setMax(const std::map<int, uint64_t>& m) {
    if (m.empty()) {
        throw std::runtime_error("Map is empty");
    }
    int maxKey = std::numeric_limits<int>::min();
    for (const auto& [key, value] : m) {
        if (key > maxKey) {
            maxKey = key;
        }
    }
    return maxKey;
}

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


