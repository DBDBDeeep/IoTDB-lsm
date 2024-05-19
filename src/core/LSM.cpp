
#include "LSM.h"


bool LSM::isDelayData(uint64_t key){
    //비어있을때도 start key는 있음
//    if(activeNormalMemtable->mem.empty()){ //비어있을때는 delay data가 아니다.
//        return false;
//    }else{
//        return activeNormalMemtable->mem.begin()->first > key;
//
//    }

    return activeNormalMemtable->startKey > key;

}

void LSM::insertData(IMemtable& memtable, uint64_t key, int value){

    if (memtable.isFull()) {
        try {
            IMemtable* newMemtable = transformActiveToImm(&memtable);
            if(newMemtable->startKey>key){  //delay data
                insertData(*activeDelayMemtable, key, value);
            }else{ //normal data
                newMemtable->setStartKey(key);
                newMemtable->put(key, value);
            }

            return;
        } catch (exception &e) {
            cerr << e.what() << "\n";
        }
    }
    memtable.put(key, value);

    return;
}

void LSM::insert(uint64_t key, int value){
    if(!isDelayData(key)) {
        insertData(*activeNormalMemtable, key, value);
    }
    else {
        insertData(*activeDelayMemtable, key, value);
    }
}



int LSM::readData(uint64_t key){

    for (auto imm : immMemtableList) {
        // 맵에서 키 검색
        auto it = imm->mem.find(key);
        if (it != imm->mem.end()) {
            cout<<"(found in id:"<<imm->memtableId<<")";
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    return diskRead(key); //빈함수
}

int LSM::diskRead(uint64_t key){
    cout<<"reading Disk data~";
    disk->readCount++;

    return disk->read(key);
}

map<uint64_t, int> LSM::range(uint64_t start, uint64_t end){

    // 로깅 ====
    list<string> ids;
    bool flag;
    // ======

    map<uint64_t, int> results;
    for (auto imm : immMemtableList) {
        flag = false;
        for (const auto& entry : imm->mem) {
            if (entry.first >= start && entry.first <= end) {
                results[entry.first] = entry.second;
                flag = true;
            }
        }
        if(flag) ids.push_back("("+to_string(imm->memtableId)+")");
    }

    // 만약 start 범위가 disk일 가능성이 있을때
    map<uint64_t, int> diskData;

    if(results.empty() || start < results.begin()->first
                       || end !=results.rbegin()->first){
        diskData=diskRange(start, end);
    }

    if(!ids.empty()){
        cout << "found in immMemtables ";
        for (auto id: ids) cout << id;
        cout <<"\n";
    }

    //병합
    results.insert(diskData.begin(), diskData.end());

    return results;
}

map<uint64_t, int> LSM::diskRange(uint64_t start, uint64_t end){
    cout<<"ranging Disk datas~ ";
    map<uint64_t, int> diskData = disk->range( start, end);
    disk->readCount += diskData.size();

    return diskData;
}


IMemtable* LSM::transformActiveToImm(IMemtable* memtable) {

    if(immMemtableList.size()==memtableNum){
        flush();
    }

    uint64_t minKey = numeric_limits<uint64_t>::max();
    uint64_t maxKey = numeric_limits<uint64_t>::min();

    auto updateKeys = [&](auto memtablePtr) {
        for (const auto& entry : memtablePtr->mem) {
            if (entry.first < minKey) minKey = entry.first;
            if (entry.first > maxKey) maxKey = entry.first;
        }
        memtablePtr->setState(IMM);
        if (minKey != numeric_limits<uint64_t>::max()) {
            memtablePtr->setStartKey(minKey);
        }
        if (maxKey != numeric_limits<uint64_t>::min()) {
            memtablePtr->setLastKey(maxKey);
        }
        immMemtableList.push_back(memtablePtr);
    };

    if (auto normalPtr = dynamic_cast<NormalMemtable*>(memtable)){
        updateKeys(activeNormalMemtable);
        activeNormalMemtable = new NormalMemtable(++currentId);
        activeNormalMemtable->setStartKey(maxKey);
        return dynamic_cast<NormalMemtable*>(activeNormalMemtable);
    } else if (auto delayPtr = dynamic_cast<DelayMemtable*>(memtable)){
        updateKeys(activeDelayMemtable);
        activeDelayMemtable = new DelayMemtable(++currentId);
        return dynamic_cast<DelayMemtable*>(activeDelayMemtable);
    } else {
        throw logic_error("transformActiveToImm 주소비교.. 뭔가 문제가 있는 듯 하오.");
    }
}

int LSM::flush(){

    int flag=0; //0: delay, 1: normal

    IMemtable* flushMemtable=immMemtableList.front();

    if(immMemtableList.front()->type==NI){
        flag=1;
    }

    // unordered_map에서 데이터 추출
    vector<std::pair<uint64_t, int>> sortedData(flushMemtable->mem.begin(), flushMemtable->mem.end());

    // vector를 정렬
    sort(sortedData.begin(), sortedData.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    //파일 만들기
    makeFile(sortedData, flag);

    disk->flush(flushMemtable);

    immMemtableList.pop_front();

    return 0;

}
void LSM::printActiveMemtable(bool printKV){
    auto findMinMaxKeys = [](const auto& memMap) {
        uint64_t minKey = std::numeric_limits<uint64_t>::max();
        uint64_t maxKey = std::numeric_limits<uint64_t>::min();
        for (const auto& pair : memMap) {
            if (pair.first < minKey) minKey = pair.first;
            if (pair.first > maxKey) maxKey = pair.first;
        }
        return std::make_pair(minKey, maxKey);
    };

    cout << "\n=======print Active Nomal Memtable=====\n";
    cout << "Memtable Id: " << activeNormalMemtable->memtableId << ", "
         << "Memtable Size: " << activeNormalMemtable->getSize() << ", "
         << "total data : " << activeNormalMemtable->mem.size() << "\n";

    if (!activeNormalMemtable->mem.empty()) {
        auto keys = findMinMaxKeys(activeNormalMemtable->mem);
        cout << "(startKey: " << keys.first << ", "
             << "lastKey: " << keys.second << ")\n";
    }
    if (printKV) {
        for (const auto &pair: activeNormalMemtable->mem) {
            cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
        }
    }
    cout << "\n";

    cout << "\n======print Active Delay Memtable======\n";
    cout << "Memtable Id: " << activeDelayMemtable->memtableId << ", "
         << "Memtable Size: " << activeDelayMemtable->getSize() << ", "
         << "total data: " << activeDelayMemtable->mem.size() << "\n";
    if (!activeDelayMemtable->mem.empty()) {
        auto keys = findMinMaxKeys(activeDelayMemtable->mem);
        cout << "(startKey: " << keys.first << ", "
             << "lastKey: " << keys.second << ")\n";
    }
    if (printKV) {
        for (const auto &pair: activeDelayMemtable->mem) {
            cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
        }
    }
    cout<<"\n";

    return;

}


void LSM::printImmMemtable(){

    cout<<"\n============ImmMemtable===========\n";
    for (auto imm: immMemtableList) {
        string state = (typeid(*imm) == typeid(NormalMemtable))?"normal" : "delay";
        cout << "[ " << state << " id(" << imm->memtableId << ") ]  key: " << imm->startKey << " ~ " << imm->lastKey
             << " | #: " << imm->mem.size() << "\n";
    }

    cout<<"\n";

    return;

}
