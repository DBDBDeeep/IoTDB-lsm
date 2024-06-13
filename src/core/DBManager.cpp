
#include "DBManager.h"


bool DBManager::isDelayData(uint64_t key){

    return activeNormalMemtable->startKey > key;

}

void DBManager::insertData(IMemtable& memtable, uint64_t key, int value){

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

void DBManager::insert(uint64_t key, int value){
    if(!isDelayData(key)) {
        insertData(*activeNormalMemtable, key, value);
    }
    else {
        insertData(*activeDelayMemtable, key, value);
    }
}



int DBManager::readData(uint64_t key){

    for (auto imm : immMemtableList) {
        // 맵에서 키 검색
        auto it = imm->mem.find(key);
        if (it != imm->mem.end()) {
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    return DiskRead(key); //빈함수
}

int DBManager::DiskRead(uint64_t key){

    int value= Disk->read(key);
    if(value!=NULL){
        diskReadCnt++;
        diskReadData++;
    }

    return value;
}

map<uint64_t, int> DBManager::range(uint64_t start, uint64_t end){

    map<uint64_t, int> results;

    // unordered_map을 스캔하여 범위 내 데이터 추출
    if(Nmin > end || Nmax < start){ //Normal Imm 안읽어도됨
        
        if(Dmin > end || Dmax < start){ //delay Imm 안읽어도됨


        }else{

            for (auto imm : immMemtableList) {
                if(imm->type==DI) continue;
                if(imm->startKey > end || imm->lastKey < start)
                    continue;

                for (const auto& entry : imm->mem) {
                    if (entry.first >= start && entry.first <= end) {
                        results[entry.first] = entry.second;
                        //flag = true;
                    }
                }

            }
            
        }


    }else{  //그냥 다 읽자

        for (auto imm : immMemtableList) {
            if(imm->startKey > end || imm->lastKey < start)
                continue;

            for (const auto& entry : imm->mem) {
                if (entry.first >= start && entry.first <= end) {
                    results[entry.first] = entry.second;

                }
            }
   
        }
        
    }
    

    // 만약 start 범위가 Disk일 가능성이 있을때
    map<uint64_t, int> DiskData;
    if(results.size()!=(end-start+1)){
        DiskData = DiskRange(start, end);
        results.insert(DiskData.begin(), DiskData.end());
    }

    return results;
}


map<uint64_t, int> DBManager::DiskRange(uint64_t start, uint64_t end){

    map<uint64_t, int> DiskData = Disk->range(start, end);
    
    if(!DiskData.empty()){
        diskReadCnt++;
        diskReadData+=DiskData.size();
    }

    return DiskData;
}


IMemtable* DBManager::transformActiveToImm(IMemtable* memtable) {

    if(immMemtableList.size()>=memtableNum){ //예스스레드

        IMemtable* flushMemtable=immMemtableList.front();
        if(flushMemtable->type == NI) {
                flushQueue.push(immMemtableList.front());
                immMemtableList.pop_front();
                deleteMem(Nstart, flushMemtable->memtableId);
                deleteMem(Nlast, flushMemtable->memtableId);
                Nmin=setMin(Nstart);
                Nmax=setMax(Nlast);

            flushController->start(N); //예스스레드

        }
        else if(flushMemtable->type == DI) {

                flushQueue.push(immMemtableList.front());
                immMemtableList.pop_front();
                deleteMem(Dstart, flushMemtable->memtableId);
                deleteMem(Dlast, flushMemtable->memtableId);
                Dmin=setMin(Dstart);
                Dmax=setMax(Dlast);

            flushController->start(D); //예스스레드

        }
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

        Nstart[memtable->memtableId]=memtable->startKey;
        Nlast[memtable->memtableId]=memtable->lastKey;
        Nmin=setMin(Nstart);
        Nmax=setMax(Nlast);

        return dynamic_cast<NormalMemtable*>(activeNormalMemtable);
    } else if (auto delayPtr = dynamic_cast<DelayMemtable*>(memtable)){
        updateKeys(activeDelayMemtable);
        activeDelayMemtable = new DelayMemtable(++currentId);
        Dstart[memtable->memtableId]=memtable->startKey;
        Dlast[memtable->memtableId]=memtable->lastKey;
        Dmin=setMin(Dstart);
        Dmax=setMax(Dlast);
        return dynamic_cast<DelayMemtable*>(activeDelayMemtable);
    } else {
        throw logic_error("transformActiveToImm 주소비교.. 뭔가 문제가 있는 듯 하오.");
    }
}


int DBManager::flush(){

    int flag=0; //0: delay, 1: normal

    IMemtable* flushMemtable=immMemtableList.front();
    if(flushMemtable->type == NI) {
        if(flushQueue.empty()) {
            flushQueue.push(immMemtableList.front());
            immMemtableList.pop_front();
        }
        flushController->start(N);
    }
    else if(flushMemtable->type == DI) {
        if(flushQueue.empty()) {
            flushQueue.push(immMemtableList.front());
            immMemtableList.pop_front();
        }
        flushController->start(D);
    }


    return 0;

}
void DBManager::printActiveMemtable(bool printKV){
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
    cout << "\n";

    return;
}



void DBManager::printImmMemtable(){

    cout<<"\n============ImmMemtable===========\n";
    for (auto imm: immMemtableList) {
        string state = (typeid(*imm) == typeid(NormalMemtable))?"normal" : "delay";
        cout << "[ " << state << " id(" << imm->memtableId << ") ]  key: " << imm->startKey << " ~ " << imm->lastKey
             << " | #: " << imm->mem.size() << "\n";
    }

    cout<<"\n";

    return;

}

void DBManager::makeFile(const std::vector<std::pair<uint64_t, int>>& sortedData ,int flag) {

    string filename;

    if(flag){  //normal
        filename ="../src/test/SSTable/NormalSStable" + to_string(++fileCounter) + ".txt";
    }else{  //delay
        filename ="../src/test/SSTable/DelaySStable" + to_string(++fileCounter) + ".txt";
    }

    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file: " << filename << endl;
        return;
    }
    outputFile<<sortedData.size()<<"\n";  //사이즈
    outputFile<<sortedData.begin()->first<<"\t"<<sortedData.rbegin()->first<<"\n"; //처음키, 마지막키
    for (const auto& pair : sortedData) {
        outputFile << pair.first << "\t" << pair.second << "\n";
    }
    outputFile.close();
    //cout << "Data written to " << filename << endl;
}