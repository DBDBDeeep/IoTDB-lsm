//#include "NormalMemtable.h"
#include "IMemtable.h"

NormalMemtable::NormalMemtable() {
    state = ACTIVE;
    startKey = 0;
    lastKey = static_cast<uint64_t>(-1);
}

bool NormalMemtable::isFull() {
    size_t currentSize = mem.size() * (sizeof(uint64_t) + sizeof(int));
    return currentSize >= memtableSize;
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
