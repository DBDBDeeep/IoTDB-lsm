#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <map>

enum State{
    ACTIVE, IMM
};

// Abstract class for memtables
class IMemtable {
public:
    std::map<uint64_t, int> mem;
    State state;
    uint64_t startKey;
    uint64_t lastKey;
//    size_t memtableSize = 16 * 1024 * 1024;
    size_t memtableSize = 4 * 1024; // delay == normal ?
    int memtableId = 0;

    virtual ~IMemtable() = default;
    virtual bool isFull() = 0;
    virtual void put(uint64_t key, int value) = 0;
    virtual bool setState(State state) = 0;
    virtual bool setStartKey(uint64_t key) = 0;
    virtual bool setLastKey(uint64_t key) = 0;
    virtual size_t getSize() = 0;
};

class NormalMemtable : public IMemtable {
public:
    NormalMemtable(int memtableId);
    bool isFull() override;
    void put(uint64_t key, int value) override;
    bool setState(State state) override;
    bool setStartKey(uint64_t key) override;
    bool setLastKey(uint64_t key) override;
    size_t getSize();
};

class DelayMemtable : public IMemtable {
public:
    DelayMemtable(int memtableId);
    bool isFull() override;
    bool setState(State state) override;
    void put(uint64_t key, int value) override;
    bool setStartKey(uint64_t key) override;
    bool setLastKey(uint64_t key) override;
    size_t getSize();
};

#endif // MEMTABLE_H
