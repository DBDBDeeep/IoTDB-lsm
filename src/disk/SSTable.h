#ifndef SSTABLE_H
#define SSTABLE_H

#include <cstddef> // size_t를 위한 표준 라이브러리 포함
#include <iostream>
#include <map>

using namespace std;


enum Type {
    N, D
};

struct SSTable {
public:
    unordered_map<uint64_t, int> ss;
    Type type;
    uint64_t startKey;
    uint64_t lastKey;
    //size_t sstableSize = 16 * 1024 * 1024;
    size_t sstableSize = 4 * 1024;
    int sstableId;

    SSTable(int id); // 생성자 선언
    void setType(Type type);
    bool isFull();
    void put(uint64_t key, int value);
    bool setStartKey(uint64_t key);
    bool setLastKey(uint64_t key);
    static void printSStable(SSTable *table);
};

#endif // SSTABLE_H
