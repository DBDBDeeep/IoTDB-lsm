#ifndef IOTDB_LSM_OPERATORMANAGER_H
#define IOTDB_LSM_OPERATORMANAGER_H

#include "ActiveMemtableController.h"
#include "ImmutableMemtableController.h"

extern ActiveMemtableController* activeMemtableController;
extern ImmutableMemtableController* immMemtableController;


class OperatorManager {
public:
    OperatorManager(){
        activeMemtableController = new ActiveMemtableController();
        immMemtableController = new ImmutableMemtableController();
    }
    bool insert(unsigned int key, int value);
    int readData(unsigned int key);
    map<unsigned int, int> range(unsigned int start, unsigned int end);
    bool isFull(IMemtable& memtable);
    IMemtable* transforActiveToImm(IMemtable& memtable); // normal? delay?

};


#endif //IOTDB_LSM_OPERATORMANAGER_H
