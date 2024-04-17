
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



// TODO : Imm으로 고쳐라..
int LSM::readData(unsigned int key){
    auto active_mem_it = activeNormalMemtable->normalMem.find(key);
    if (active_mem_it != activeNormalMemtable->normalMem.end()) {
        activeNormalMemtable->access++;
        return active_mem_it->second;
    }
    auto delay_mem_it = activeDelayMemtable->delayMem.find(key);
    if (delay_mem_it != activeDelayMemtable->delayMem.end()) {
        return delay_mem_it->second;
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

map<unsigned int, int> LSM::range(unsigned int start, unsigned int end){
    // active, delay에서 찾기
    map<unsigned int, int> normalResultMap(activeNormalMemtable->normalMem.lower_bound(start), activeNormalMemtable->normalMem.upper_bound(end));
    map<unsigned int, int> delayResultMap(activeDelayMemtable->delayMem.lower_bound(start), activeDelayMemtable->delayMem.upper_bound(end));

    if(normalResultMap.empty() && delayResultMap.empty()) {
        diskRead(start);
        throw empty("no data!!\n");
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
    }
    else if(&memtable == activeDelayMemtable) {
        immDelayMemtableList.emplace_back(activeDelayMemtable);
        return activeDelayMemtable = new DelayMemtable();
    }
    else
        throw logic_error("transforActiveToImm 주소비교.. 뭔가 문제가 있는 듯 하오.");
}