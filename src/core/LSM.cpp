
#include "LSM.h"


bool LSM::isDelayData(uint64_t key){
    if(activeNormalMemtable->mem.empty()){ //비어있을때는 delay data가 아니다.
        return false;
    }else{
        return activeNormalMemtable->mem.begin()->first > key;

    }

}

void LSM::insertData(IMemtable& memtable, uint64_t key, int value){

    if (memtable.isFull()) {
        try {
            IMemtable* newMemtable = transformActiveToImm(&memtable);
            newMemtable->setStartKey(key);
            newMemtable->put(key, value);
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
        for (auto it = imm->mem.lower_bound(start); it != imm->mem.end() && it->first <= end; ++it) {
            results[it->first] = it->second;
            flag = true;
        }
        if(flag) ids.push_back("("+to_string(imm->memtableId)+")");
    }

    // 만약 start 범위가 disk일 가능성이 있을때
    map<uint64_t, int> diskData;

    if(start<results.begin()->first){
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



IMemtable* LSM::transformActiveToImm(IMemtable* memtable){  //0 normal 1 delay


    if(immMemtableList.size()==memtableNum){
        flush();
    }

    if (auto normalPtr = dynamic_cast<NormalMemtable*>(memtable)){
        activeNormalMemtable->setState(IMM);
        activeNormalMemtable->setStartKey(activeNormalMemtable->mem.begin()->first);
        activeNormalMemtable->setLastKey(activeNormalMemtable->mem.rbegin()->first);
        immMemtableList.push_back(activeNormalMemtable);
        activeNormalMemtable = new NormalMemtable(++currentId);
        return dynamic_cast<NormalMemtable*>(activeNormalMemtable);

    } else if (auto delayPtr = dynamic_cast<DelayMemtable*>(memtable)){
        activeDelayMemtable->setState(IMM);
        activeDelayMemtable->setStartKey(activeDelayMemtable->mem.begin()->first);
        activeDelayMemtable->setLastKey(activeDelayMemtable->mem.rbegin()->first);
        immMemtableList.push_back(activeDelayMemtable);
        activeDelayMemtable = new DelayMemtable(++currentId);
        return dynamic_cast<DelayMemtable*>(activeDelayMemtable);
    }

    else
        throw logic_error("transformActiveToImm 주소비교.. 뭔가 문제가 있는 듯 하오.");


}

int LSM::flush(){

    IMemtable* flushMemtable=immMemtableList.front();

    disk->flush(flushMemtable);

    immMemtableList.pop_front();

    return 0;

}

void LSM::printActiveMemtable(bool printKV){

    cout<<"\n=======print Active Nomal Memtable=====\n";
    std::cout << "Memtable Id: "<< activeNormalMemtable->memtableId << ", "
            << "Memtable Size: "<< activeNormalMemtable->getSize() << ", "
              << "total data : "<< activeNormalMemtable->mem.size() << "\n";

    if(!activeNormalMemtable->mem.empty()) {
        std::cout << "(startKey: " << activeNormalMemtable->startKey << ", "
                  << "lastKey: " << activeNormalMemtable->mem.rbegin()->first << ")\n";
    }
    if(printKV) {
        for (const auto &pair: activeNormalMemtable->mem) {
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
        }
    }
    cout<<"\n";

    cout<<"\n======print Active Delay Memtable======\n";
    std::cout << "Memtable Id: "<< activeDelayMemtable->memtableId << ", "
            << "Memtable Size: "<< activeDelayMemtable->getSize() << ", "
            << "total data: "<< activeDelayMemtable->mem.size() << "\n";
    if(!activeDelayMemtable->mem.empty()) {
        std::cout << "(startKey: " << activeDelayMemtable->startKey << ", "
                  << "lastKey: " << activeDelayMemtable->mem.rbegin()->first << ")\n";
    }
    if(printKV) {
        for (const auto &pair: activeDelayMemtable->mem) {
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
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
