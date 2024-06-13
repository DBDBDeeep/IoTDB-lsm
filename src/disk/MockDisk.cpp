#include "MockDisk.h"



int MockDisk::read(uint64_t key) {
    //normal SStale 뒤지기
    for (auto ss : normalSSTables) {
        if(ss->startKey > key || ss->lastKey < key)
            continue;
        // 맵에서 키 검색

        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    //없으면 delay SStale 뒤지기
    for (auto ss : delaySSTables) {
        if(ss->startKey > key || ss->lastKey < key)
            continue;
        // 맵에서 키 검색

        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {

            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    return NULL;
}

map<uint64_t, int> MockDisk::range(uint64_t start, uint64_t end) {
    // 로깅
    list<string> normalSSTableIds;
    list<string> delaySSTableIds;

    map<uint64_t, int> results;

    for (auto ss : normalSSTables) {
        if(ss->startKey > end || ss->lastKey < start)
            continue;
        auto itStart = ss->ss.lower_bound(start); // start 이상의 첫 번째 요소를 찾음
        auto itEnd = ss->ss.upper_bound(end);     // end 이하의 마지막 요소의 다음 요소를 찾음

        for (auto it = itStart; it != itEnd; ++it) {
            results[it->first] = it->second;

        }
    }

    // Delay SSTables
    for (auto ss : delaySSTables) {
        if(ss->startKey > end || ss->lastKey < start)
            continue;

        auto itStart = ss->ss.lower_bound(start); // start 이상의 첫 번째 요소를 찾음
        auto itEnd = ss->ss.upper_bound(end);     // end 이하의 마지막 요소의 다음 요소를 찾음

        for (auto it = itStart; it != itEnd; ++it) {
            results[it->first] = it->second;
        }
    }

    return results;
}

bool MockDisk::flush(IMemtable* mem) {
    SSTable* newSSTable = new SSTable(mem->memtableId);

    for (const auto& entry : mem->mem) {
        newSSTable->put(entry.first, entry.second);
    }

    newSSTable->setStartKey(newSSTable->ss.begin()->first);
    newSSTable->setLastKey(newSSTable->ss.rbegin()->first);

    if(mem->type == NI){

        newSSTable->setType(N);
        normalSSTables.push_back(newSSTable);
        return true;
    }else if(mem->type == DI){

        newSSTable->setType(D);
        delaySSTables.push_back(newSSTable);
        return true;
    }

    return false;
}

void MockDisk::printSSTableList() {

    cout<<"\n============NormalSSTable===========\n";
    for(auto table: normalSSTables){
        cout<<"[ "<<table->sstableId<<" ]  key: " <<table->startKey<<" ~ "<<table->lastKey<<" | #cnt: "<<table->ss.size()<<"\n";
    }
    cout<<"\n============DelaySSTable===========\n";
    for(auto table: delaySSTables){
        cout<<"[ "<<table->sstableId<<" ]  key: "<<table->startKey<<" ~ "<<table->lastKey<<" | #cnt: "<<table->ss.size()<<"\n";
    }

}
