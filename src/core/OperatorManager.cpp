#include "OperatorManager.h"

ActiveMemtableController* activeMemtableController;
ImmutableMemtableController* immMemtableController;

bool OperatorManager::insert(unsigned int key, int value){

}
int OperatorManager::readData(unsigned int key){

}
map<unsigned int, int> OperatorManager::range(unsigned int start, unsigned int end){

}
bool OperatorManager::isFull(IMemtable& memtable){

}
IMemtable* OperatorManager::transforActiveToImm(IMemtable& memtable) {

}