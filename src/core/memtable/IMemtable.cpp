#include "IMemtable.h"
#include <iostream>

size_t IMemtable::getSize() {
    // 현재 map size
    size_t currentMapSize = mem.size() * (sizeof(uint64_t) + sizeof(int));
    // 객체 크기
    size_t instanceSize = sizeof(*this);
    return instanceSize + currentMapSize;
}

bool IMemtable::isFull(){
    size_t incomingDataSize = sizeof(uint64_t) + sizeof(int);
    return (getSize() + incomingDataSize) >= memtableSize;
}
bool IMemtable::put(uint64_t key, int value){
    return true;

}
bool IMemtable::setState(State state){

    return true;

}
bool IMemtable::setStartKey(uint64_t key){

    return true;

}
bool IMemtable::setLastKey(uint64_t key){

    return true;

}
