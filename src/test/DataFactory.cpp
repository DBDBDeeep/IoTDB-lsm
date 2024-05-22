
#include "DataFactory.h"


using namespace std;

//o3 데이터 없이 데이터셋 생성
void DataFactory:: generateNormalDataset(int n){
    tree = new LSM();
    for (uint64_t i = 1; i <= n; ++i) {
        auto data = make_pair(i, static_cast<int>(i * 2));
        tree->insert(data.first, data.second);
    }
}

void DataFactory:: NormalTest(){
    tree->printActiveMemtable(true);
    tree->printImmMemtable();
    tree->disk->printSSTableList();

    //parameter case별로 테스트 결과 달라짐
//    cout<<"\n\n\n\n========read/range test=========\n";
//
//    cout<<"[Read] key : "<<2009<<" value : "<<tree->readData(2009)<<"\n"; // normal ImmMemtable read
//    cout<<"[Read] key : "<<2010<<" value : "<<tree->readData(2010)<<"\n"; // normal ImmMemtable read
//    cout<<"[Read] key : "<<2011<<" value : "<<tree->readData(2011)<<"\n"; // normal ImmMemtable read
//    cout<<"[Read] key : "<<1000<<" value : "<<tree->readData(1000)<<"\n"; // normalSSTable read
//    cout<<"[Read] key : "<<1200<<" value : "<<tree->readData(1200)<<"\n"; // normalSSTable read
//    // Active에만 있는 data를 읽으려 할 때 불가! : -1
//    cout<<"[Read] key : "<<2390<<" value : "<<tree->readData(2390)<<"\n";
//
//    // 하나의 normal ImmMemtable range
//    cout<<"\n[range] 하나의 normal ImmMemtable range : 2020 ~ 2030\n";
//    map<uint64_t, int> result = tree->range(2020,2030);
//    for(auto data: result){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//    // 여러개의 normal ImmMemtable range
//    cout<<"\n[range] 여러개의 normal ImmMemtable range : 2005 ~ 2015\n";
//    map<uint64_t, int> result1 = tree->range(2005,2015);
//    for(auto data: result1){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//
//    // 한개의 SStalble range
//    cout<<"\n[range] 한개의 SStalble range : 700 ~ 710\n";
//    map<uint64_t, int> result2 = tree->range(700,710);
//    for(auto data: result2){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//
//    // 여러개의 SStalble range
//    cout<<"\n[range] 여러개의 SStalble range : 330 ~ 340\n";
//    map<uint64_t, int> result3 = tree->range(330,340);
//    for(auto data: result3){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//
//    // ImmTable과 SStable에 섞여있을때
//    cout<<"\n[range] ImmTable과 SStable에 섞여있을때 : 1670 ~ 1680\n";
//    map<uint64_t, int> result4 = tree->range(1670,1680);
//    for(auto data: result4){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
    delete tree;
}

void DataFactory:: delayedTest(){

    tree->printActiveMemtable(false);
    tree->printImmMemtable();
    tree->disk->printSSTableList();

    //parameter case별로 테스트 결과 달라짐
//    cout<<"\n\n\n\n========read/range test=========\n";
//
//    cout<<"[Read] key : "<<2009<<" value : "<<tree->readData(2009)<<"\n"; // normal ImmMemtable read
//    cout<<"[Read] key : "<<2010<<" value : "<<tree->readData(2010)<<"\n"; // normal ImmMemtable read
//    cout<<"[Read] key : "<<2011<<" value : "<<tree->readData(2011)<<"\n"; // normal ImmMemtable read
//    cout<<"[Read] key : "<<1000<<" value : "<<tree->readData(1000)<<"\n"; // normalSSTable read
//    cout<<"[Read] key : "<<1200<<" value : "<<tree->readData(1200)<<"\n"; // normalSSTable read
//    // Active에만 있는 data를 읽으려 할 때 불가! : -1
//    cout<<"[Read] key : "<<2390<<" value : "<<tree->readData(2390)<<"\n";
//
//    // 하나의 normal ImmMemtable range
//    cout<<"\n[range] 하나의 normal ImmMemtable range : 2020 ~ 2030\n";
//    map<uint64_t, int> result = tree->range(2020,2030);
//    for(auto data: result){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//    // 여러개의 normal ImmMemtable range
//    cout<<"\n[range] 여러개의 normal ImmMemtable range : 2005 ~ 2015\n";
//    map<uint64_t, int> result1 = tree->range(2005,2015);
//    for(auto data: result1){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//
//    // 한개의 SStalble range
//    cout<<"\n[range] 한개의 SStalble range : 700 ~ 710\n";
//    map<uint64_t, int> result2 = tree->range(700,710);
//    for(auto data: result2){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//
//    // 여러개의 SStalble range
//    cout<<"\n[range] 여러개의 SStalble range : 330 ~ 340\n";
//    map<uint64_t, int> result3 = tree->range(330,340);
//    for(auto data: result3){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//
//    // ImmTable과 SStable에 섞여있을때
//    cout<<"\n[range] ImmTable과 SStable에 섞여있을때 : 1670 ~ 1680\n";
//    map<uint64_t, int> result4 = tree->range(1670,1680);
//    for(auto data: result4){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
    delete tree;
};

// o3데이터 포함 데이터셋 생성 함수
void DataFactory:: generateDelayedDataset(int dataNum, double outOfOrderRatio, int numSegments) {
    vector<uint64_t> dataset;
    int outOfOrderCount = static_cast<int>(dataNum * outOfOrderRatio); // out of order 데이터 총 개수
    vector<vector<uint64_t>> outOfOrderKeysPerSegment(numSegments/2);  //segment 묶음 단위 out of order 데이터들을 저장할 벡터


    srand(time(0));
    random_device rd; // 난수 생성기 시드
    mt19937 gen(rd()); // Mersenne Twister 난수 생성기

    // segment의 크기 설정
    int remaining = outOfOrderCount / 2;
    int size;

    for (int i = 0; i < (numSegments / 2)-1; ++i) {
        do{
            size = rand() % (remaining - 4) + 2; // remaining이 2 이상이어야하고, 현재 size도 2 이상이어야함
        }while(size<=0);
        sizes.push_back(size);
        remaining -= size;
    }
    sizes.push_back(remaining); // 남은 크기를 마지막 구간에 추가

    // 1부터 outOfOrderCount/2 사이의 랜덤한 숫자 생성
    uniform_int_distribution<> dis( 1, outOfOrderCount / 2);
    int delayedKey = dis(gen); // 첫번째 delaydata 선정

    for (size_t i = 0; i < sizes.size(); i++) {
        cout << "\n구간 " << i + 1 << " (" << sizes[i] << "개) : " << delayedKey << " ~ " << delayedKey + sizes[i] - 1 << "\n";;

        for (int j = 0; j < sizes[i]; j++) {
            outOfOrderKeysPerSegment[i].push_back(delayedKey + j);
        }


        if (delayedKey + sizes[i] <= dataNum - outOfOrderCount / 2) {
            uniform_int_distribution<> dis2(delayedKey + sizes[i], dataNum - outOfOrderCount / 2);
            delayedKey = dis2(gen);
        }
        else {
            if(i == sizes.size()-1){
                break;
            }
            else{
                cout<< delayedKey << " "<< sizes[i] << " "<< dataNum << outOfOrderCount / 2<<endl;
                cerr << "ERR: 범위가 유효하지 않습니다. 루프를 종료합니다.\n";
                break;
            }

        }

    }
    vector<uint64_t> dataSet(dataNum);

    // dataSet 초기화 (1부터 dataNum까지)
    std::iota(dataSet.begin(), dataSet.end(), 1);

    // outOfOrderKeysPerSegment에 포함되는 key를 찾아 outOfOrderKeys에 저장
    unordered_set<uint64_t> outOfOrderKeys;
    for (const auto& segment : outOfOrderKeysPerSegment) {
        for (const auto& key : segment) {
            outOfOrderKeys.insert(key);
        }
    }

    // outOfOrderKeys에 포함되는 key를 제외한 나머지 key를 dataSet에 저장
    dataSet.erase(
            remove_if(dataSet.begin(), dataSet.end(), [&](uint64_t key) {
                return outOfOrderKeys.find(key) != outOfOrderKeys.end();
            }),
            dataSet.end()
    );

    // 단일 out of order data 선정하여 randomKeys에 저장
    vector<uint64_t> remainingKeys(dataSet.begin(), dataSet.end());
    srand(static_cast<unsigned>(std::time(0))); // 난수 생성기 초기화

    std::unordered_set<uint64_t> randomKeys;
    size_t numberOfKeysToSelect = outOfOrderCount / 2;

    while (randomKeys.size() < numberOfKeysToSelect) {
        size_t randomIndex = rand() % remainingKeys.size();
        randomKeys.insert(remainingKeys[randomIndex]);
    }

    dataSet.erase(
            remove_if(dataSet.begin(), dataSet.end(), [&](uint64_t key) {
                return randomKeys.find(key) != randomKeys.end();
            }),
            dataSet.end()
    );

    // 각 구간에 단일 out of order data 추가
    for(const auto &key : randomKeys) {
        int randomChoice = rand() % 100 + 1;

        if (randomChoice <= 30) {
            randomIndex = key + (rand() % 500 + 1); // 1~500 범위 내
        } else if (randomChoice <= 60) {
            randomIndex = key + (rand() % 500 + 501); // 501~1000 범위 내
        } else if (randomChoice <= 80) {
            randomIndex = key + (rand() % 1000 + 1001); // 1001~2000 범위 내
        } else {
            randomIndex = rand() % dataSet.size() + 1 + key;
        }
//        cout<<randomIndex<<endl;

        // 랜덤 인덱스가 dataSet 범위를 벗어나면 dataSet 뒤에 추가
        if(randomIndex>= dataSet.size()){
            dataSet.push_back(key);
        }else{
            dataSet.insert(dataSet.begin() + randomIndex, key);

        }
    }


    // 각 구간에 out of order segment를 추가
    for(const auto &segment : outOfOrderKeysPerSegment){
        //현재 segment의 마지막 key값보다 큰 인덱스에 랜덤 생성
        //TODO: offset 설정하기
        int randomChoice = rand() % 100 + 1;
//        cout<<"randomChoice : "<<randomChoice<<endl;
//        cout<<"segment.back() : "<<segment.back()<<endl;

        int isIndexValid;
        if (randomChoice <= 30) {
            randomIndex = rand() % 500 + 1; // 1~500 범위 내
            isIndexValid = segment.back() - segment.front() + randomIndex;
        } else if (randomChoice <= 60) {
            randomIndex = rand() % 500 + 501; // 501~1000 범위 내
            isIndexValid = segment.back() - segment.front() + randomIndex;
        } else if (randomChoice <= 80) {
            randomIndex = rand() % 1000 + 1001; // 1001~2000 범위 내
            isIndexValid = segment.back() - segment.front() + randomIndex;
        } else {
            randomIndex = rand() % dataSet.size() + 1; // 1 ~ dataSet.size() 범위 내
            isIndexValid = segment.back() - segment.front() + randomIndex;
        }
//        cout<<randomIndex<<endl;

        // 랜덤 인덱스가 dataSet 범위를 벗어나면 dataSet 뒤에 추가
        if(isIndexValid>= dataSet.size()){
            for (const auto& key : segment) {
                dataSet.push_back(key);
//                cout<<"push_back\n"<<key<<" ";
            }
        }
        else{
            for (const auto& key : segment) {
                dataSet.insert(dataSet.begin() + randomIndex++, key);
//                cout<<"push\n"<<key<<" ";
            }
        }
    }


    // 데이터셋이 비어있지 않을 때에만 파일에 쓰기
    if (!dataSet.empty()) {
        writeToInitFile("../src/test/dataset/initData.txt", dataSet);
    } else {
        cerr << "ERR: initData.txt가 비어있습니다." << endl;
    }
}

void DataFactory::writeToInitFile(string filePath, vector<uint64_t>& dataset) {
    ofstream outputFile(filePath);
    std::cout << ">> Write to Init File Progress \n";

    if (!outputFile.is_open()) {
        cerr << "ERR: workload dataset 파일 열기 오류" << endl;
        return;
    }
    const uint64_t progressInterval = dataset.size() / 10; // 1%마다 진행률 출력
    for(int i=0; i<dataset.size(); i++){
        outputFile << "INSERT," << dataset[i] << endl;
        if (i % progressInterval == 0) {
            std::cout << "Progress: " << (i * 100 / dataset.size()) << "%\n";
        }
    }

    cout<<"100% file write완료\n";
    outputFile.close();
}



//파일에 워크로드 데이터를 쓰는 함수
void DataFactory::writeToWorkloadFile(string filePath, vector<Record>& dataset) {
    ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        cerr << "workload dataset 파일 열기 오류" << endl;
        return;
    }
    const uint64_t progressInterval = dataset.size() / 10; // 1%마다 진행률 출력
//    for(int i=0; i<dataset.size(); i++){
//        outputFile << "INSERT," << dataset[i] << endl;
//        if (i % progressInterval == 0) {
//            std::cout << "Progress: " << (i * 100 / dataset.size()) << "%\n";
//        }
//    }
    std::cout << ">> Write to Workload File Progress \n";
    for(int i=0; i<dataset.size(); i++){
        if (strcmp(dataset[i].op.c_str(), "RANGE")==0) {
            outputFile << dataset[i].op << "," << dataset[i].start_key << " " << dataset[i].end_key << endl;
        } else {
            outputFile << dataset[i].op << "," << dataset[i].key << endl;
        }

        if (i % progressInterval == 0) {
            cout << (i * 100 / dataset.size()) << "%\n";
        }
    }
    cout<<"100% file write완료\n";
    outputFile.close();
}
// workload 데이터셋 생성 함수
void DataFactory::generateWorkloadDataset(vector<Record>& initDataSet, string filePath, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion) {

    vector<Record> dataset;
    int initFileRecordCount = initDataSet.size();   //전체 데이터셋 개수
    int txnFileRecordCount = initFileRecordCount/2; // 워크로드의 INSERT 작업 개수(전체 데이터 셋의 절반)
    int singleReadCount = txnFileRecordCount * (readProportion/insertProportion) * singleReadProportion; // 단일 읽기 작업 해야할 총 횟수
    int rangeCount = txnFileRecordCount * (readProportion/insertProportion) * rangeProportion; // 범위 조회 작업 해야할 총 횟수

    for(int i=0; i<initFileRecordCount; i++){
            Record record;
            record.key = initDataSet[i].key;
            record.op = "INSERT";
            dataset.push_back(record);
    }
    while (singleReadCount > 0 || rangeCount > 0) {
            // 랜덤한 인덱스에 read 작업 레코드 추가
            int randomReadKey = rand() % (initFileRecordCount/2) + 1;
            Record record;
            if (singleReadCount > 0) {                          // single read 작업일 경우
                record.key = randomReadKey;      // 랜덤한 키 선택
                --singleReadCount;
                record.op = "READ";
            } else {                                            // range read 작업일 경우
                int rangeStart = rand() % (initFileRecordCount/2) + 1;
                int rangeEnd = rand() % (initFileRecordCount/2) + 1;
                if (rangeStart > rangeEnd) {
                    swap(rangeStart, rangeEnd); // 오름차순으로 정렬
                }
                record.start_key = rangeStart;
                record.end_key = rangeEnd;
                record.op = "RANGE";
                --rangeCount;
            }
            //Todo: offset설정해서 적용
            dataset.insert(dataset.begin() +initFileRecordCount/2 + randomReadKey, record); // 워크로드 데이터셋 랜덤한 위치에 삽입
    }

    // 파일에 생성된 워크로드 데이터셋 쓰기
    writeToWorkloadFile(filePath, dataset);


    return;
}


void DataFactory::writeToFile(size_t bytes){

    ofstream file(filename, ios::binary);
    vector<char> data(bytes);

    if(!file.is_open()){
        cerr << "ERR: 파일 open 오류\n";
    }

    // 데이터를 무작위로 생성
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255);

    for (size_t i = 0; i < bytes; ++i) {
        data[i] = static_cast<char>(distrib(gen));
    }

    auto start = chrono::high_resolution_clock::now();
    file.write(data.data(), data.size());
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed = end - start;
    cout << "Write time: " << elapsed.count() << " ms" << endl;

    file.close();

}
void DataFactory::readFromFile(size_t bytes){

    ifstream file(filename, ios::binary);
    vector<char> data(bytes);

    auto start = chrono::high_resolution_clock::now();
    file.read(data.data(), bytes);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed = end - start;
    cout << "Read time: " << elapsed.count() << " ms" << endl;

    file.close();

}


void DataFactory::printDelayData(){

    int delaySSTableNum= tree->disk->delaySSTables.size();
    int delaySSTableSize=0;
    if(delaySSTableNum!= 0){
        delaySSTableSize= tree->disk->delaySSTables.front()->ss.size();
    }

    cout<<"the number of delay data in Disk : "<<delaySSTableNum*delaySSTableSize<<"\n";

    int delayImmMemtableNum=0;
    int delayActiveMemtableNum=tree->activeDelayMemtable->mem.size();
    for(auto memtable : tree->immMemtableList){
        if(memtable->type=='D') delayImmMemtableNum++;
    }

    cout<<"the number of delay data in Memory : "<< delayImmMemtableNum*delaySSTableSize+delayActiveMemtableNum<<"\n";

}
