

#include "DataFactory.h"


using namespace std;

/**
 * InitDataset에 추가할 Segment단위 delay data 생성 함수
 * */
void DataFactory::generateDelaySegments(std::vector<std::vector<int>>& outOfOrderKeysPerSegment, int dataNum, int numOfSegments) {

    std::random_device rd;
    std::mt19937 g(rd());
    /**out of order data 각 segment 크기 설정
     * */
    int count25Percent = std::round(numOfSegments * 0.25);
    int count50Percent = std::round(numOfSegments * 0.50);
    int totalAssigned = 2 * count25Percent + count50Percent;

    // 비율에 따라 세그먼트 크기 추가
    for (int i = 0; i < count25Percent; ++i) {
        sizes.push_back(10);
    }

    for (int i = 0; i < count25Percent; ++i) {
        sizes.push_back(30);
    }

    for (int i = 0; i < count50Percent; ++i) {
        sizes.push_back(20);
    }

    // 생성된 segment 개수와 총 segment개수 불일치할 경우 계산 보정
    if(totalAssigned < numOfSegments) {
        for (int i = 0; i < numOfSegments - totalAssigned; ++i) {
            sizes.push_back(20); // 남은 부분을 20으로 채웁니다. 필요시 다른 값으로 대체 가능
        }
    } else if(totalAssigned > numOfSegments) {
        sizes.resize(numOfSegments); // 초과한 부분을 잘라냅니다.
    }

    std::shuffle(sizes.begin(), sizes.end(), g);



    /**out of order data segment에 들어갈 key 선정
     * */
    int distance = dataNum / numOfSegments; // delay segment 생성될 구간 설정
    int start=1;
    int end=distance;
    int delayedKey;
    for (size_t i = 0; i < sizes.size(); i++) {
        std::uniform_int_distribution<> nextDis(start, end-sizes[i]); // currentIdx 부터 distance-(생성될 segment 사이즈) 사이에서 seg에 들어갈 첫번째 delay key선정
        delayedKey = nextDis(g);

        // std::cout << "\n구간 " << i + 1 << " (" << sizes[i] << "개) : " << delayedKey << " ~ " << delayedKey + sizes[i] - 1 << "\n";

        for (int j = 0; j < sizes[i]; j++) {
            outOfOrderKeysPerSegment[i].push_back(delayedKey + j);
        }

        start = end+1;
        end = start+distance;

    }
}
/**
 * InitDataset에 추가할 Segment단위 delay data 생성 함수
 * */
unordered_multimap<int, uint64_t> DataFactory::generateSingleDelayDataset(unordered_set<uint64_t>& remainingKeys, vector<uint64_t>& dataSet) {
    /**out of order 단일 key 선정
   * */

    srand(static_cast<unsigned>(std::time(0))); // 난수 생성기 초기화

    std::unordered_multimap<int, uint64_t> randomKeys;
    size_t numberOfKeysToSelect = outOfOrderCount / 2;
    int offset=0;
    while (randomKeys.size() < numberOfKeysToSelect) {
        size_t randomDelayKey = rand() % remainingKeys.size(); //randomKeys의 key로 사용
        //value
       offset = setSingleDelayOffset(randomDelayKey, dataSet);
       randomKeys.insert({randomDelayKey, offset});
    }
    return randomKeys;
}

int DataFactory::setSingleDelayOffset(int key, vector<uint64_t>& dataSet) {
    int randomChoice = rand() % 100 + 1;
    if (randomChoice <= 30) {
        randomIndex = key + (rand() % 500 + 1); // 1~500 범위 내
    } else if (randomChoice <= 60) {
        randomIndex = key + (rand() % 500 + 501); // 501~1000 범위 내
    } else if (randomChoice <= 80) {
        randomIndex = key + (rand() % 1000 + 1001); // 1001~2000 범위 내
    } else {
        randomIndex = rand() % dataSet.size() + key;
    }

   return randomIndex;
}

/** Delay Segment Offset 설정 함수
 * */
int DataFactory::setSegmentDelayOffset(const vector<int> &segment, size_t dataSetSize) {
    //현재 segment의 마지막 key값보다 큰 인덱스에 랜덤 생성(offset)
    int randomChoice = rand() % 100 + 1;
    int isIndexValid=0;
    int randomIndex=0;

    //(30%, 30%, 20%, 20%) 비율에 따라 delay되는 offset 설정
    if (randomChoice <= 30) {
        randomIndex = rand() % 500 + 1; // 1~500 범위 내
        isIndexValid = segment.back() + randomIndex;
    } else if (randomChoice <= 60) {
        randomIndex = rand() % 500 + 501; // 501~1000 범위 내
        isIndexValid = segment.back() + randomIndex;
    } else if (randomChoice <= 80) {
        randomIndex = rand() % 1000 + 1001; // 1001~2000 범위 내
        isIndexValid = segment.back()  + randomIndex;
    } else {
        randomIndex = rand() % dataSetSize + 1; // 1 ~ dataSet.size() 범위 내
        isIndexValid = segment.back() + randomIndex;
    }
    return isIndexValid;

}



void DataFactory::generateO3Dataset(string& dataSetName, int dataNum, double outOfOrderRatio){
    int outOfOrderCount = static_cast<int>(dataNum * outOfOrderRatio); // out of order 데이터 총 개수
    int segmentDataNum= outOfOrderCount/2;
    int numOfSegments= segmentDataNum/20;  //하이퍼파라미터
    std::vector<std::vector<int>> outOfOrderKeysPerSegment(numOfSegments);


    generateDelaySegments(outOfOrderKeysPerSegment, dataNum, numOfSegments);

    /**1~dataNum 범위 dataset 초기화
    * */
    vector<uint64_t> dataSet(dataNum);
    std::iota(dataSet.begin(), dataSet.end(), 1);

    /**out of order segment에 포함되는 key들 찾아 dataSet에서 제거
     * */
    unordered_set<uint64_t> outOfOrderKeys;
    for (const auto& segment : outOfOrderKeysPerSegment) {
        for (const auto& key : segment) {
            outOfOrderKeys.insert(key);
        }
    }

    dataSet.erase(
            remove_if(dataSet.begin(), dataSet.end(), [&](uint64_t key) {
                return outOfOrderKeys.find(key) != outOfOrderKeys.end();
            }),
            dataSet.end()
    );
    //single delay key 생성
    std::unordered_set<uint64_t> remainKeys(dataSet.begin(), dataSet.end());
    std::unordered_multimap<int, uint64_t> randomKeys = generateSingleDelayDataset(remainKeys, dataSet);
    //생성된 single delay key를 기존 dataset에서 제거(중복 key 방지)
    dataSet.erase(
            remove_if(dataSet.begin(), dataSet.end(), [&](uint64_t key) {
                return randomKeys.find(key) != randomKeys.end();
            }),
            dataSet.end()
    );



    int lineToWrite = dataNum;
    writeToInitFile(dataSetName, dataSet, randomKeys, outOfOrderKeysPerSegment, lineToWrite);

}
/** Delay dataSet 생성 함수*/
/**void DataFactory:: generateDelayedDataset(string& dataSetName, int dataNum, double outOfOrderRatio) {
    int outOfOrderCount = static_cast<int>(dataNum * outOfOrderRatio); // out of order 데이터 총 개수
    int segmentDataNum= outOfOrderCount/2;
    int numOfSegments= segmentDataNum/20;  //하이퍼파라미터


//    std::vector<int> indices(segmentDataNum);
//    std::iota(indices.begin(), indices.end(), 0);
//
//    // Randomly shuffle the indices
//    std::random_device rd;
//    std::mt19937 g(rd());
//    std::shuffle(indices.begin(), indices.end(), g);

    std::vector<std::vector<int>> outOfOrderKeysPerSegment(numOfSegments); //segment 묶음 단위 out of order 데이터들을 저장할 벡터


    //1~dataNum 범위 dataset 초기화

    deque<uint64_t> dataSet(dataNum);
    std::iota(dataSet.begin(), dataSet.end(), 1);



    //out of order data 각 segment 크기 설정

    int count25Percent = std::round(numOfSegments * 0.25);
    int count50Percent = std::round(numOfSegments * 0.50);
    int totalAssigned = 2 * count25Percent + count50Percent;

    // 비율에 따라 세그먼트 크기 추가
    for (int i = 0; i < count25Percent; ++i) {
        sizes.push_back(10);
    }

    for (int i = 0; i < count25Percent; ++i) {
        sizes.push_back(30);
    }

    for (int i = 0; i < count50Percent; ++i) {
        sizes.push_back(20);
    }

    // 생성된 segment 개수와 총 segment개수 불일치할 경우 계산 보정
    if(totalAssigned < numOfSegments) {
        for (int i = 0; i < numOfSegments - totalAssigned; ++i) {
            sizes.push_back(20); // 남은 부분을 20으로 채웁니다. 필요시 다른 값으로 대체 가능
        }
    } else if(totalAssigned > numOfSegments) {
        sizes.resize(numOfSegments); // 초과한 부분을 잘라냅니다.
    }

    std::shuffle(sizes.begin(), sizes.end(), g);



    //out of order data segment에 들어갈 key 선정

    int distance = dataNum / numOfSegments; // delay segment 생성될 구간 설정
    int start=1;
    int end=distance;
    int delayedKey;
    for (size_t i = 0; i < sizes.size(); i++) {
        std::uniform_int_distribution<> nextDis(start, end-sizes[i]); // currentIdx 부터 distance-(생성될 segment 사이즈) 사이에서 seg에 들어갈 첫번째 delay key선정
        delayedKey = nextDis(g);

        // std::cout << "\n구간 " << i + 1 << " (" << sizes[i] << "개) : " << delayedKey << " ~ " << delayedKey + sizes[i] - 1 << "\n";

        for (int j = 0; j < sizes[i]; j++) {
            outOfOrderKeysPerSegment[i].push_back(delayedKey + j);
        }

        start = end+1;
        end = start+distance;

    }



    //out of order segment에 포함되는 key들 찾아 dataSet에서 제거

    unordered_set<uint64_t> outOfOrderKeys;
    for (const auto& segment : outOfOrderKeysPerSegment) {
        for (const auto& key : segment) {
            outOfOrderKeys.insert(key);
        }
    }

    dataSet.erase(
            remove_if(dataSet.begin(), dataSet.end(), [&](uint64_t key) {
                return outOfOrderKeys.find(key) != outOfOrderKeys.end();
            }),
            dataSet.end()
    );


    //out of order 단일 key 선정

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


    //dataset에 단일 out of order data 추가

    // cout<<"\n>> 단일 Out of order data 추가\n\n";
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

        // 랜덤 인덱스가 dataSet 범위를 벗어나면 dataSet 뒤에 추가
        if(randomIndex >= dataSet.size()){
            dataSet.push_back(key);
        }else{
            dataSet.insert(dataSet.begin() + randomIndex, key);
        }

        iteration++;

        if ( iteration % (randomKeys.size() / 100) == 0) {
            VECTOR_LOG_PROGRESS(iteration, randomKeys);
        }
    }

    //<<"단일키"<<randomKeys.size()<<endl;
    // dataset에 out of order data segment 추가

    //  cout<<">> Out of order dataSet 추가\n\n";

    if (outOfOrderKeysPerSegment.empty()) {
        cerr << "ERR: outOfOrderKeysPerSegment is empty." << endl;
        return;
    }

    iteration=0;
    for(const auto &segment : outOfOrderKeysPerSegment){
        // segment가 비어있는지 확인
        if (segment.empty()) {
            cerr << "ERR: segment is empty." << endl;
            continue;
        }

        if (segment.back() <= 0) {
            cerr << "ERR: delay segment size 생성이 제대로 되지 않았습니다.\n size is " << segment.back() << endl;
            continue;
        }

        //segment가 들어갈 offset 설정
        size_t dataSetSize = dataSet.size();
        int offsetIndex = setSegmentDelayOffset(segment,dataSetSize);


        // 각 delay segment를 offset에 따라 dataSet에 추가
        if (offsetIndex >= static_cast<int>(dataSet.size())) {
            for (const auto& key : segment) {
                dataSet.push_back(key);
            }
        } else {
            for (const auto& key : segment) {
                dataSet.insert(dataSet.begin() + offsetIndex, key);
                ++offsetIndex;  // 증가 연산자 위치 수정
            }
        }

        iteration++;
        if (outOfOrderKeysPerSegment.size() > 0 && (outOfOrderKeysPerSegment.size() / 100) > 0) {
            if (iteration % (outOfOrderKeysPerSegment.size() / 100) == 0) {
                INT_LOG_PROGRESS(iteration, outOfOrderKeysPerSegment.size());
            }
        }

    }

    // File에 쓰기

    string filePath = "../src/test/dataset/"+dataSetName+ "_c"+to_string(dataNum)+"_d"+to_string(outOfOrderRatio).substr(0, std::to_string(outOfOrderRatio).find('.') + 2)+".txt";
    if (!dataSet.empty()) {
        writeToInitFile(filePath, dataSet);
    } else {
        cerr << "ERR:"<<filePath <<"가 비어있습니다." << endl;
    }
}
**/


/**
 * double형을 string으로 변환하는 함수
 * */
std::string to_string_with_precision(double value, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << value;
    return out.str();
}
/**
 * opertor<< 오버로딩 함수
 * */
std:: ostream& operator<<(std::ostream& os, const Record& record) {
    if(record.op=="RANGE"){
        os << "op: " << record.op << ", start_key: " << record.start_key << ", end_key: " << record.end_key;
    }else{
        os << "op: " << record.op << ", key: " << record.key;
    }
    return os;
}
/**
 * Workload에 추가할 Read, Range 작업 생성 함수
 * */
void DataFactory::generateReadRangeDataset(string initDataName, string& workloadDataName, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion, list<Record>& initDataSet, list<Record>& initTxnSet) {
    cout<<"start"<<endl;

    int initFileRecordCount = initDataSet.size();
    int singleReadCount = initFileRecordCount * (readProportion / insertProportion) * singleReadProportion;
    int rangeCount = initFileRecordCount * (readProportion / insertProportion) * rangeProportion;
    //SingleRead 총 작업 생성
    for(int i=0; i< singleReadCount; i++){
        randomReadKey = rand() % initFileRecordCount + 1;
        Record record;
        record.key = randomReadKey;
        record.op = "READ";
        singleReadSet.insert({randomReadKey, record});
    }
    //RangeRead 총 작업 생성
    for(int i=0; i<rangeCount; i++) {
        int rangeStart = rand() % initFileRecordCount + 1;
        int rangeEnd = rand() % initFileRecordCount + 1;
        if (rangeStart > rangeEnd) {
            std::swap(rangeStart, rangeEnd);
        }
        Record record;
        record.start_key = rangeStart;
        record.end_key = rangeEnd;
        record.op = "RANGE";
        rangeSet.insert({rangeStart, record});
    }
    cout<<"end"<<endl;
}



void DataFactory::transferLinesToWorkloadFile(const std::string &initFilePath, int linesToRead) {
    //read용 dataset input file 열기
    std::ifstream file(initFilePath.c_str());
    if (!file.is_open()) {
        cerr << "ERR: 파일을 열 수 없습니다 " << initFilePath << endl;
        return;
    }
    //write용 workload dataset output file 열기
    std::string outputFilePath = "../src/test/dataset/workload/new_workload.txt";
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "ERR: workload 파일 열기 오류" << outputFilePath << std::endl;
        return;
    }

    std::string line;
    int lineCount = 0;
    std::string op, key, start_key, end_key;
    while (lineCount < linesToRead && std::getline(file, line)) {
        lineCount++;
    }
    lineCount=0;
    cout<< "generate workload Progress"<<endl;
    while (lineCount <= linesToRead && std::getline(file, line)) {
        std::istringstream iss(line);
        //input file에서 한 줄 읽어오기
        if (std::getline(iss, op, ',') && std::getline(iss, key)) {
            outputFile << op << "," << stoull(key) << std::endl;
        }
        //lineCount와  singleReadSet, rangeSet의 key를 비교하여 일치하는 경우 workload dataset에 추가
        if (singleReadSet.find(lineCount) != singleReadSet.end()) {
            //outputFile에 singleReadSet의 record.op와 record.key를 쓰기
            outputFile << "READ" << "," << lineCount << std::endl;
        } else if (rangeSet.find(lineCount) != rangeSet.end()) {
            outputFile << "RANGE" << "," << rangeSet[lineCount].start_key << " " << rangeSet[lineCount].end_key
                       << std::endl;
        }

        lineCount++;
        if(lineCount % (linesToRead / 10) == 0){
            INT_LOG_PROGRESS(lineCount, linesToRead);
        }
    }
}


void DataFactory::writeToInitFile(string filePath, vector<uint64_t>& dataset, unordered_multimap<int, uint64_t>& singleRandomKeys, unordered_set<uint64_t>& segmentRandomKeys, int lineToWrite){
    ofstream outputFile(filePath);
    int lineCount = 0;
    std::cout << "\n>> Write to Init File Progress \n\n";

    if (!outputFile.is_open()) {
        cerr << "ERR: workload dataset 파일 열기 오류"  << endl;
        return;
    }
    std::cout << ">> Write to Init File Progress \n\n";
    while(lineCount <= lineToWrite){
        outputFile << "INSERT," << dataset[lineCount] << std::endl;
        lineCount++;
        //singleRandomKeys의 key와 lineCount가 일치하는 경우 파일에 key에 해당하는 singleRandomeKeys의 value값쓰기
        if (singleRandomKeys.find(lineCount) != singleRandomKeys.end()) {
            outputFile << "INSERT," << singleRandomKeys.find(lineCount)->second << std::endl;
            //singleRandomKeys의 key,val값을 dataset에서 제거
            singleRandomKeys.erase(lineCount);
        }
        if (lineCount % (lineToWrite / 10) == 0) {
            INT_LOG_PROGRESS(lineCount, lineToWrite);
        }
    }
    //singleRandomKeys의 요소가 남아있다면, key값을 모두 파일에 쓰기
    for (const auto& key : singleRandomKeys) {
        outputFile << "INSERT," << key.second << std::endl;
    }




    cout<<"filewrite완료\n";
    outputFile.close();
}


/**File에 Workload 쓰기 함수*/
//void DataFactory::writeToWorkloadFile(const std::string& filePath, std::list<Record>& dataset) {
//    std::ofstream outputFile(filePath);
//    if (!outputFile.is_open()) {
//        std::cerr << "workload dataset 파일 열기 오류" << std::endl;
//        return;
//    }
//    std::cout << "\n>> Write to Workload File Progress \n";
//    size_t datasetSize = dataset.size();
//    iteration = 0;
//
//    for (const auto& record : dataset) {
//        if (record.op == "RANGE") {
//            outputFile << record.op << "," << record.start_key << " " << record.end_key << std::endl;
//        } else {
//            outputFile << record.op << "," << record.key << std::endl;
//        }
//        iteration++;
//        if ((iteration) % datasetSize == 0) {
//            INT_LOG_PROGRESS(iteration, datasetSize);
//        }
//    }
//
//    outputFile.close();
//}