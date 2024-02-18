#include "LSM.h"
#include "Memtable.cpp"

NormalMemtable* activeNormalMemtable;
DelayMemtable* activeDelayMemtable;

bool LSM::isFull(Memtable& memtable){
    return memtable.isFull();
}

void LSM::insertData(unsigned int key, int value){

}

int LSM::readData(unsigned int key){

}

int LSM::diskRead(unsigned int key){

}

map<unsigned int, int> LSM::range(unsigned int start, unsigned int end){

}

bool LSM::convertActiveToImm(Memtable& memtable){
    return memtable.setState(IMM);
}
