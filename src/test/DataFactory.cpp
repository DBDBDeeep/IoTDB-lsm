

#include "DataFactory.h"


using namespace std;

/** Delay dataSet 생성 함수*/
void DataFactory:: generateDelayedDataset(string& dataSetName, int dataNum, double outOfOrderRatio) {
    int outOfOrderCount = static_cast<int>(dataNum * outOfOrderRatio); // out of order 데이터 총 개수
    int segmentDataNum= outOfOrderCount/2;
    int numOfSegments= segmentDataNum/20;  //하이퍼파라미터


    std::vector<int> indices(segmentDataNum);
    std::iota(indices.begin(), indices.end(), 0);

    // Randomly shuffle the indices
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    std::vector<std::vector<int>> outOfOrderKeysPerSegment(numOfSegments); //segment 묶음 단위 out of order 데이터들을 저장할 벡터


    /**1~dataNum 범위 dataset 초기화
     * */
    deque<uint64_t> dataSet(dataNum);
    std::iota(dataSet.begin(), dataSet.end(), 1);



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


    /**out of order 단일 key 선정
     * */
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


    /** dataset에 단일 out of order data 추가
     * */
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
    /** dataset에 out of order data segment 추가
     * */
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
        int offsetIndex = setDelaySegmentOffset(segment,dataSetSize);


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

    /** File에 쓰기
    * */
    string filePath = "../src/test/dataset/"+dataSetName+ "_c"+to_string(dataNum)+"_d"+to_string(outOfOrderRatio).substr(0, std::to_string(outOfOrderRatio).find('.') + 2)+".txt";
    if (!dataSet.empty()) {
        writeToInitFile(filePath, dataSet);
    } else {
        cerr << "ERR:"<<filePath <<"가 비어있습니다." << endl;
    }
}

/** Delay Segment Offset 설정 함수
 * */
int DataFactory::setDelaySegmentOffset(const vector<int> &segment, size_t dataSetSize) {
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

std::string to_string_with_precision(double value, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << value;
    return out.str();
}



/** Workload 데이터 생성 함수*/
void DataFactory::generateWorkloadDataset(string initDataName, string& workloadDataName, double readProportion, double insertProportion, double singleReadProportion, double rangeProportion, list<Record>& initDataSet, list<Record>& initTxnSet) {

    int initFileRecordCount = initDataSet.size();
    int singleReadCount = initFileRecordCount * (readProportion / insertProportion) * singleReadProportion;
    int rangeCount = initFileRecordCount * (readProportion / insertProportion) * rangeProportion;
    std::cout << ">> Generate to Workload Dataset Progress \n\n";

    for(int i=0; i< singleReadCount; i++){
        randomReadKey = rand() % initFileRecordCount + 1;
        Record record;
        record.key = randomReadKey;
        record.op = "READ";
        auto it = std::next(initTxnSet.begin(), randomReadKey);
        initTxnSet.insert(it, record);

//        cout<<"single read 진행률 : "<<i<<"/"<<singleReadCount<<endl;
        if (i != 0 && i % (singleReadCount / 100) == 0) {
            INT_LOG_PROGRESS(i, singleReadCount);
        }
    }
    for(int i=0; i<rangeCount; i++){
        int rangeStart = rand() % initFileRecordCount + 1;
        int rangeEnd = rand() % initFileRecordCount + 1;
        if (rangeStart > rangeEnd) {
            std::swap(rangeStart, rangeEnd);
        }
        Record record;
        record.start_key = rangeStart;
        record.end_key = rangeEnd;
        record.op = "RANGE";
        auto it = std::next(initTxnSet.begin(),  rangeStart);
        initTxnSet.insert(it, record);
//        cout<<"range 진행률 : "<<i<<"/"<<rangeCount<<endl;
        if (i != 0 && i % (rangeCount / 100) == 0) {
            INT_LOG_PROGRESS(i, rangeCount);
        }
    }



    /**파일에 쓰기*/
    std::string filePath;
    if(singleReadProportion == 0.5) {
        filePath = "../src/test/dataset/workload/"+workloadDataName+"_i" + to_string_with_precision(insertProportion, 1) +
                   "_r" + to_string_with_precision(readProportion, 1) + "_V1_"+initDataName+".txt";
    } else {
        filePath = "../src/test/dataset/workload/"+workloadDataName+"_r"+ to_string_with_precision(readProportion, 1) +
                   "_i" + to_string_with_precision(insertProportion, 1) + "_V2_"+initDataName+".txt";
    }
    writeToWorkloadFile(filePath, initTxnSet);

    return;
}

void DataFactory::writeToInitFile(string filePath, deque<uint64_t>& dataset) {
    ofstream outputFile(filePath);
    std::cout << "\n>> Write to Init File Progress \n\n";

    if (!outputFile.is_open()) {
        cerr << "ERR: workload dataset 파일 열기 오류" << endl;
        return;
    }
    int datasetSize = dataset.size();
    for(int i=0; i<datasetSize; i++){
        outputFile << "INSERT," << dataset[i] << endl;
        if (i != 0 && i % (dataset.size() / 100) == 0) {
            VECTOR_LOG_PROGRESS(i, dataset);
        }
    }

    outputFile.close();
}



/**File에 Workload 쓰기 함수*/
void DataFactory::writeToWorkloadFile(const std::string& filePath, std::list<Record>& dataset) {
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cerr << "workload dataset 파일 열기 오류" << std::endl;
        return;
    }
    std::cout << "\n>> Write to Workload File Progress \n";
    size_t datasetSize = dataset.size();
    iteration = 0;

    for (const auto& record : dataset) {
        if (record.op == "RANGE") {
            outputFile << record.op << "," << record.start_key << " " << record.end_key << std::endl;
        } else {
            outputFile << record.op << "," << record.key << std::endl;
        }
        iteration++;
        if ((iteration) % datasetSize == 0) {
            INT_LOG_PROGRESS(iteration, datasetSize);
        }
    }

    outputFile.close();
}
