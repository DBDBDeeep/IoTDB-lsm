
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
            memtable.put(key, value);
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
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    return diskRead(key); //빈함수
}

int LSM::diskRead(uint64_t key){
    cout<<"reading Disk data~ \n";
    disk->readCount++;

    return disk->read(key);
}

map<uint64_t, int> LSM::range(uint64_t start, uint64_t end){

    map<uint64_t, int> results;
    for (auto imm : immMemtableList) {
        for (auto it = imm->mem.lower_bound(start); it != imm->mem.end() && it->first <= end; ++it) {
            results[it->first] = it->second;
        }
    }

    // 만약 start 범위가 disk일 가능성이 있을때
    map<uint64_t, int> diskData;
    if(start<results.begin()->first){
        diskData=diskRange(start, end);
    }

    //병합
    results.insert(diskData.begin(), diskData.end());

    return results;
}

map<uint64_t, int> LSM::diskRange(uint64_t start, uint64_t end){
    cout<<"reading Disk datas~ \n";
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
        throw logic_error("transforActiveToImm 주소비교.. 뭔가 문제가 있는 듯 하오.");


}

int LSM::flush(){

    IMemtable* flushMemtable=immMemtableList.front();

    disk->flush(flushMemtable);

    immMemtableList.pop_front();

    return 0;

}

void LSM::printActiveMemtable(){

    cout<<"\n=======print Active Nomal Memtable=====\n";
    for(const auto& pair:activeNormalMemtable->mem){
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
    }
    cout<<"\n";

    cout<<"\n======print Active Delay Memtable======\n";
    for(const auto& pair:activeDelayMemtable->mem){
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
    }
    cout<<"\n";

    return;

}


void LSM::printImmMemtable(){

    cout<<"\n============ImmMemtable===========\n";
    for(auto imm: immMemtableList){
        string state = (imm->state == 1) ? "normal" : "delay";
        cout << "[ " << state << " ]  key: " << imm->startKey << " ~ " << imm->lastKey << " | #: " << imm->mem.size() << "\n";
    }

    cout<<"\n";

    return;

}
