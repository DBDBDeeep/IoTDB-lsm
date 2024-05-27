#include "IMemtable.h"

DelayMemtable::DelayMemtable(int memtableId) {
    this->state = ACTIVE;
    this->type = DI;
    this->startKey = 0;
    this->lastKey = static_cast<uint64_t>(-1);
    this->memtableId = memtableId;
}

size_t DelayMemtable::getSize() {
    size_t currentMapSize = mem.size() * (sizeof(uint64_t) + sizeof(int));
    return sizeof(*this) + currentMapSize;
}

bool DelayMemtable::isFull () {
    size_t incomingDataSize = sizeof(uint64_t) + sizeof(int);
    return (getSize() + incomingDataSize) >= memtableSize*0.8;
}

bool DelayMemtable::setState(State newState) {
    state = newState;
    return true;
}

bool DelayMemtable::put(uint64_t key, int value) {
    return mem.insert({key, value}).second;
}

bool DelayMemtable::setStartKey(uint64_t key) {
    startKey = key;
    return true;
}

bool DelayMemtable::setLastKey(uint64_t key) {
//    if (!mem.empty()) {
//        lastKey = (--mem.end())->first;
//    } else {
//        lastKey = 0; // Or any other default value to indicate an empty memtable
//    }
    lastKey=key;
    return true;
}

int DelayMemtable::getDataNum(){
    return mem.size();
}