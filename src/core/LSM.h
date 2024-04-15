#include <iostream>
#include "Memtable.cpp"
#include "../disk/Disk.cpp"

using namespace std;

extern NormalMemtable* activeNormalMemtable;
extern DelayMemtable* activeDelayMemtable;
extern vector<NormalMemtable*> immNormalMemtableList;
extern vector<DelayMemtable*> immDelayMemtableList;
extern Disk* disk;

class LSM {
public:
    LSM() {
        activeNormalMemtable = new NormalMemtable();
        activeDelayMemtable = new DelayMemtable();
    }

//    bool isFull(IMemtable& memtable);
    void insert(unsigned int key, int value);
    void insertData(IMemtable& memtable, unsigned int key, int value);
    int readData(unsigned int key);
    int diskRead(unsigned int key);
    int diskRange(unsigned int start, unsigned int end);
    map<unsigned int, int> range(unsigned int start, unsigned int end);
    IMemtable* transforActiveToImm(IMemtable& memtable); // normal? delay?
};
