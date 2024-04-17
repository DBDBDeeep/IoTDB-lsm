
#include "LSM.h"


bool LSM::isDelayData(uint64_t key){
    if(activeNormalMemtable->mem.empty()){ //비어있을때는 delay data가 아니다.
        return false;
    }else{
        return activeNormalMemtable->mem.begin()->first > key;

    }

}

void LSM::insertData(IMemtable& memtable, unsigned int key, int value){
    if (memtable.isFull()) {
        try {
            memtable = *transforActiveToImm(memtable);
            memtable.setStartKey(key);
        } catch (exception &e) {
            cerr << e.what() << "\n";
        }
    }
    memtable.put(key, value);
}

void LSM::insert(unsigned int key, int value){
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

int LSM::diskRead(unsigned int key){
    cout<<"reading Disk data~ \n";
    // TODO : disk 훑기
    disk->readCount++;
    return -1;
}

int LSM::diskRange(unsigned int start, unsigned int end){
    cout<<"reading Disk datas~ \n";
    // TODO : disk 훑기
    disk->readCount += 10;
    return -1;
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
