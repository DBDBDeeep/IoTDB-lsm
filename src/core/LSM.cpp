
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
            IMemtable* newMemtable = transforActiveToImm(&memtable);
            newMemtable->setStartKey(key);
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

    // access 추가
    activeNormalMemtable->access += normalResultMap.size();


    // 병합
    map<unsigned int, int> resultMap;
    resultMap.insert(normalResultMap.begin(), normalResultMap.end());
    resultMap.insert(delayResultMap.begin(), delayResultMap.end());

    return resultMap;
map<uint64_t, int> LSM::diskRange(uint64_t start, uint64_t end){
    cout<<"reading Disk datas~ \n";
    map<uint64_t, int> diskData = disk->range( start, end);
    disk->readCount += diskData.size();

    return diskData;
}

IMemtable* LSM::transforActiveToImm(IMemtable& memtable){
    memtable.setState(IMM);
    memtable.setLastKey();
    // TODO : 대충함. 주소비교방식으로.. 클래스비교방식으로 시도해보든가
    if(&memtable == activeNormalMemtable) {
        immNormalMemtableList.emplace_back(activeNormalMemtable);
        return activeNormalMemtable = new NormalMemtable();
IMemtable* LSM::transforActiveToImm(IMemtable* memtable){  //0 normal 1 delay


    if(immMemtableList.size()==memtableNum){
        flush();
    }

    if (auto normalPtr = dynamic_cast<NormalMemtable*>(memtable)){
        activeNormalMemtable->setState(IMM);
        activeNormalMemtable->setStartKey(activeNormalMemtable->mem.begin()->first);
        activeNormalMemtable->setLastKey(activeNormalMemtable->mem.rbegin()->first);
        immMemtableList.push_back(activeNormalMemtable);
        activeNormalMemtable = new NormalMemtable();
        return dynamic_cast<NormalMemtable*>(activeNormalMemtable);

    } else if (auto delayPtr = dynamic_cast<DelayMemtable*>(memtable)){
        activeDelayMemtable->setState(IMM);
        activeDelayMemtable->setStartKey(activeDelayMemtable->mem.begin()->first);
        activeDelayMemtable->setLastKey(activeDelayMemtable->mem.rbegin()->first);
        immMemtableList.push_back(activeDelayMemtable);
        activeDelayMemtable = new DelayMemtable();
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

