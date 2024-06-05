#include "MockDisk.h"


bool MockDisk::compaction() {
    //Todo: 구현
    return true;
}

int MockDisk::read(uint64_t key) {
    //normal SStale 뒤지기
    for (auto ss : normalSSTables) {
        if(ss->startKey > key || ss->lastKey < key)
            continue;
        // 맵에서 키 검색
//        DBManager->readCount++;
        readCount++;
//        cout<<"read"<<endl;
        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
            //        cout<<"(found in normalSSTable:"<<ss->sstableId<<")";
            return it->second;  // 키를 찾았으면 값 반환
        }

    }

    //없으면 delay SStale 뒤지기
    for (auto ss : delaySSTables) {
        if(ss->startKey > key || ss->lastKey < key)
            continue;
        // 맵에서 키 검색
        readCount++;
//        cout<<"read"<<endl;
        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
            //        cout<<"(found in delaySStale:"<<ss->sstableId<<")";
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    //없어요
    return -1;
}

map<uint64_t, int> MockDisk::range(uint64_t start, uint64_t end) {
    // 로깅
    list<string> normalSSTableIds;
    list<string> delaySSTableIds;

    map<uint64_t, int> results;

    for (auto ss : normalSSTables) {
        if(end< ss->startKey && start > ss->lastKey)
            continue;
        readCount++;
//        cout<<"read"<<endl;
        auto itStart = ss->ss.lower_bound(start); // start 이상의 첫 번째 요소를 찾음
        auto itEnd = ss->ss.upper_bound(end);     // end 이하의 마지막 요소의 다음 요소를 찾음

        for (auto it = itStart; it != itEnd; ++it) {
            results[it->first] = it->second;

        }
    }

    // Delay SSTables
    for (auto ss : delaySSTables) {
        if(ss->startKey < start || ss->lastKey > end)
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
        cout << "flush N : " <<mem->memtableId<< endl;
        newSSTable->setType(N);
        normalSSTables.push_back(newSSTable);
        return true;
    }else if(mem->type == DI){
        cout << "flush D : " <<mem->memtableId<< endl;
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
