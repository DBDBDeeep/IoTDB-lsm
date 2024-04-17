#include "Disk.h"


bool Disk::compaction() {
    //Todo: 구현
    return true;
}

int Disk::read(uint64_t key) {

    //normal SStale 뒤지기
    for (auto ss : normalSSTables) {
        // 맵에서 키 검색
        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    //없으면 delay SStale 뒤지기
    for (auto ss : delaySSTables) {
        // 맵에서 키 검색
        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    //없어요
    return -1;
}

map<uint64_t, int> Disk::range(uint64_t start, uint64_t end) {

    map<uint64_t, int> results;
    for (auto ss : normalSSTables) {
        for (auto it = ss->ss.lower_bound(start); it != ss->ss.end() && it->first <= end; ++it) {
            results[it->first] = it->second;
        }
    }
    for (auto ss : delaySSTables) {
        for (auto it = ss->ss.lower_bound(start); it != ss->ss.end() && it->first <= end; ++it) {
            results[it->first] = it->second;
        }
    }
    return results;
}

bool Disk::flush(IMemtable* mem) {
    SSTable* newSSTable = new SSTable(mem->memtableId);

    for (const auto& entry : mem->mem) {
        newSSTable->put(entry.first, entry.second);
    }

    newSSTable->setStartKey(newSSTable->ss.begin()->first);
    newSSTable->setLastKey(newSSTable->ss.rbegin()->first);

    if (auto normalPtr = dynamic_cast<NormalMemtable*>(mem)) {
        newSSTable->setType(N);
        normalSSTables.push_back(newSSTable);

    } else if (auto delayPtr = dynamic_cast<DelayMemtable*>(mem)) {
        newSSTable->setType(D);
        delaySSTables.push_back(newSSTable);
    }


    return true;
}

void Disk::printSSTableList() {

    cout<<"\n============NormalSSTable===========\n";
    int i=1;
    for(auto table: normalSSTables){
        cout<<"[ "<<i++<<" ]  key: " <<table->startKey<<" ~ "<<table->lastKey<<" | #: "<<table->ss.size()<<"\n";
    }
    cout<<"\n============DelaySSTable===========\n";
    i=1;
    for(auto table: delaySSTables){
        cout<<"[ "<<i++<<" ]  key: "<<table->startKey<<" ~ "<<table->lastKey<<" | #: "<<table->ss.size()<<"\n";
    }

}
