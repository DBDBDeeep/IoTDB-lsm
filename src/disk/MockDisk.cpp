#include "MockDisk.h"


bool MockDisk::compaction() {
    //Todo: 구현
    return true;
}

int MockDisk::read(uint64_t key) {

    //normal SStale 뒤지기
    for (auto ss : normalSSTables) {
        // 맵에서 키 검색
        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
    //        cout<<"(found in normalSSTable:"<<ss->sstableId<<")";
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    //없으면 delay SStale 뒤지기
    for (auto ss : delaySSTables) {
        // 맵에서 키 검색
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
    //bool flag;

    // Normal SSTables
    // for (auto ss : normalSSTables) {
    //     flag = false;
    //     for (const auto& entry : ss->ss) {
    //         if (entry.first >= start && entry.first <= end) {
    //             results[entry.first] = entry.second;
    //             flag = true;
    //         }
    //     }
    //     if (flag) normalSSTableIds.push_back("(" + to_string(ss->sstableId) + ")");
    // }

    for (auto ss : normalSSTables) {
        if(ss->startKey < start || ss->lastKey > end)
            continue;

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
    // 로깅
    // if (!normalSSTableIds.empty()) {
    // //    cout << "found in normalSSTables ";
    //     for (auto id : normalSSTableIds) cout << id;
    // //    cout << "\n";
    // }
    // if (!delaySSTableIds.empty()) {
    // //    cout << "found in delaySSTables ";
    // //    for (auto id : delaySSTableIds) cout << id;
    // //    cout << "\n";
    // }

    return results;
}

bool MockDisk::flush(IMemtable* mem) {
    SSTable* newSSTable = new SSTable(mem->memtableId);

    // uint64_t minKey = std::numeric_limits<uint64_t>::max();
    // uint64_t maxKey = std::numeric_limits<uint64_t>::min();

    for (const auto& entry : mem->mem) {
        newSSTable->put(entry.first, entry.second);
        // if (entry.first < minKey) {
        //     minKey = entry.first;
        // }
        // if (entry.first > maxKey) {
        //     maxKey = entry.first;
        // }
    }

    // if (minKey != std::numeric_limits<uint64_t>::max()) {
    //     newSSTable->setStartKey(minKey);
    // }
    // if (maxKey != std::numeric_limits<uint64_t>::min()) {
    //     newSSTable->setLastKey(maxKey);
    // }

    if (auto normalPtr = dynamic_cast<NormalMemtable*>(mem)) {
        newSSTable->setType(N);
        normalSSTables.push_back(newSSTable);

    } else if (auto delayPtr = dynamic_cast<DelayMemtable*>(mem)) {
        newSSTable->setType(D);
        delaySSTables.push_back(newSSTable);
    }

    return true;
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
