#include "IMemtable.h"

NormalMemtable::NormalMemtable(int memtableId) {
    this->state = ACTIVE;
    this->startKey = 0;
    this->lastKey = static_cast<uint64_t>(-1);
    this->memtableId = memtableId;
}

size_t NormalMemtable::getSize() {
    size_t currentMapSize = mem.size() * (sizeof(uint64_t) + sizeof(int));
    return sizeof(*this) + currentMapSize;
}

bool NormalMemtable::isFull() {
    size_t incomingDataSize = sizeof(uint64_t) + sizeof(int);
    return (getSize() + incomingDataSize) >= memtableSize;
}

void NormalMemtable::put(uint64_t key, int value) {
    mem.insert({key, value});
}

bool NormalMemtable::setState(State state) {
    this->state = state;
    return true;
}

bool NormalMemtable::setStartKey(uint64_t key) {
    startKey = key;
    return true;
}

bool NormalMemtable::setLastKey(uint64_t key) {
//    if (!mem.empty()) {
//        lastKey = (--mem.end())->first;
//    }

    lastKey=key;
    return true;
}