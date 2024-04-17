#include "SSTable.h"

SSTable::SSTable() : type(N), startKey(0), lastKey(0) {
    // 초기화 리스트를 사용하여 멤버 변수를 초기화합니다.
}

void SSTable::setType(Type type) {
    this->type = type;
}

bool SSTable::isFull() {
    // SSTable이 가득 찼는지 여부를 계산합니다.
    return false;
}

void SSTable::put(uint64_t key, int value) {

    ss.insert({key, value});
}

bool SSTable::setStartKey(uint64_t key) {
    startKey = key;
    return true;
}

bool SSTable::setLastKey(uint64_t key) {
    // 마지막 키를 설정하는 로직을 구현합니다.
    lastKey=key;
    return true;
}

void SSTable::printSStable(SSTable *table) {

    cout<<"print Active Delay Memtable\n";
    for(const auto& pair:table->ss){
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << "\n";
    }
    cout<<"\n";

}
