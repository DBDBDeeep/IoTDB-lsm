#include "IMemtable.h"

DelayMemtable::DelayMemtable() {
    state = ACTIVE;
    startKey = 0;
    lastKey = static_cast<uint64_t>(-1);
}

size_t DelayMemtable::getSize() {
    size_t currentMapSize = mem.size() * (sizeof(uint64_t) + sizeof(int));
    return sizeof(*this) + currentMapSize;
}

bool DelayMemtable::isFull () {
    size_t incomingDataSize = sizeof(uint64_t) + sizeof(int);
    return (getSize() + incomingDataSize) >= memtableSize;
}

bool DelayMemtable::setState(State newState) {
    state = newState;
    return true;
}

void DelayMemtable::put(uint64_t key, int value) {
    mem.insert({key, value});
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
