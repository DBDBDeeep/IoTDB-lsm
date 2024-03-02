#include "LSM.h"
#include "Memtable.cpp"

NormalMemtable* activeNormalMemtable;
DelayMemtable* activeDelayMemtable;
Disk* disk;

bool LSM::isFull(Memtable& memtable){
    return memtable.isFull();
}

bool isDelayData(unsigned int key) {
    return activeNormalMemtable->normalMem.begin()->first <= key;
}

void LSM::insertData(Memtable& memtable, unsigned int key, int value){
    if (isFull(memtable)) {
        try {
            convertActiveToImm(memtable);
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

bool LSM::convertActiveToImm(Memtable& memtable){
    memtable.setState(IMM);

}
