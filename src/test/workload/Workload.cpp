#include "Workload.h"

using namespace std;
/**
 *
 * ToDo
 *  fileUtil 파일로 빼기 => 5~148 LINE
 *
 *
 *
 *
 *
 *
 * */

int Workload::extractHalfLinesFromFilename(const string& filePath) {
    std::regex regexPattern(R"(_c(\d+))");
    std::smatch match;
    if (std::regex_search(filePath, match, regexPattern)) {
        int lineCount = std::stoi(match[1].str());
        return lineCount / 2;
    } else {
        cerr << "ERR: 파일 이름에서 줄 수를 추출할 수 없습니다: " << filePath << endl;
        return 0;
    }
}

void Workload::readLines(std::ifstream& file, std::list<Record>& dataset, int linesToRead) {
    std::string line;
    int lineCount = 0;
    cout<<"file 읽는 중\n";
    while (std::getline(file, line) && (linesToRead == -1 || lineCount < linesToRead)) {
        std::istringstream iss(line);
        std::string op;

        if (std::getline(iss, op, ',')) {
            Record record;
            record.op = op;
            if (op == "RANGE") {
                std::string startKeyStr, endKeyStr;
                if (std::getline(iss, startKeyStr, ' ') && std::getline(iss, endKeyStr)) {
                    record.start_key = std::stoull(startKeyStr);
                    record.end_key = std::stoull(endKeyStr);
                    if(record.end_key-record.start_key>=5000){
                        std::cerr << "range 범위 오류" << line << std::endl;
                        continue;
                    }
                } else {
                    std::cerr << "ERR: 잘못된 형식의 RANGE 레코드입니다: " << line << std::endl;
                    continue;
                }
            } else {
                std::string keyStr;
                if (std::getline(iss, keyStr)) {
                    record.key = std::stoull(keyStr);
                } else {
                    std::cerr << "ERR: 잘못된 형식의 " << op << " 레코드입니다: " << line << std::endl;
                    continue;
                }
            }
            lineCount++;
            dataset.push_back(record);
        } else {
            std::cerr << "ERR: 잘못된 형식의 레코드입니다: " << line << std::endl;
        }

    }
}

list<Record> Workload::readFileFromStart(const std::string &filePath, int linesToRead) {
    std::list<Record> readFileDataset;
    ifstream file(filePath.c_str());
    if (!file.is_open()) {
        cerr << "ERR: 파일을 열 수 없습니다 " << filePath << endl;
        return readFileDataset;
    }
    readLines(file, readFileDataset, linesToRead);
    file.close();
    return readFileDataset;
}

//list<Record> Workload::readFileFromMiddle(const string& filePath, int linesToRead) {
//    std::list<Record> readFileDataset;
//    ifstream file(filePath.c_str());
//    if (!file.is_open()) {
//        cerr << "ERR: 파일을 열 수 없습니다 " << filePath << endl;
//        return readFileDataset;
//    }
//
//    int skipLines = extractHalfLinesFromFilename(filePath);
//    file.clear();
//    file.seekg(0, ios::beg);
//
//    string line;
//    for (int i = 0; i < skipLines && getline(file, line); ++i) {
//        // Skip the first half lines
//    }
//
//    readLines(file, readFileDataset, linesToRead);
//    file.close();
//    return readFileDataset;
//}

list<Record> Workload::readFileWhole(const string& filePath) {
    std::list<Record> readFileDataset;
    ifstream file(filePath.c_str());
    if (!file.is_open()) {
        cerr << "ERR: 파일을 열 수 없습니다 " << filePath << endl;
        return readFileDataset;
    }
    readLines(file, readFileDataset, -1); // Read all lines
    file.close();
    return readFileDataset;
}

void Workload::executeInsertWorkload(list<Record>& dataset, int start, int end) {
    iteration = 0;
    for (const auto& record : dataset) {
        tree->insert(record.key, record.key * 2);
        iteration++;

    }

}

void Workload::executeMixedWorkload(list<Record>& dataset, int start, int end) {
    // << "Workload Mixed 작업 실행 시작\n";
    iteration=0;
    for (const auto& record : dataset) {

        if (record.op == "READ") {
            tree->readData(record.key);
        } else if (record.op == "RANGE") {
            tree->range(record.start_key, record.end_key);
        } else if (record.op == "INSERT") {
            tree->insert(record.key, record.key * 2);
        } else {
            cerr << "ERR: 잘못된 형식의 레코드입니다: " << record.op << endl;
            return;
        }
        /** 진행률 출력 */
        iteration++;

    }

    //cout << "Workload Mixed 작업 실행 끝\n";
}


void Workload::executeWorkload(list<Record>& dataset, bool isMixedWorkload) {
   // cout << "workload 실행 시작\n";
    if(!isMixedWorkload){
        executeInsertWorkload(dataset, 0, dataset.size());
        return;
    }else {
        auto start = chrono::high_resolution_clock::now();
        executeMixedWorkload(dataset, 0, dataset.size());
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "Workload 실행 시간: " << elapsed.count() << " ms" << endl;
        return;
    }
    //cout << "workload 실행 끝\n";
}


DBManager* Workload::getTree() {
    return tree;
}
void Workload::cleanup() {
    delete tree;
    tree = nullptr;
}

void Workload::deleteAllSSTable() {
    std::string directoryPath = "/home/haena/DBDBDeep/IoTDB-lsm/src/test/SSTable"; 
    try {
        // 디렉터리 내의 모든 파일 순회
        for (const auto& entry : filesystem::directory_iterator(directoryPath)) {
            filesystem::remove(entry.path()); // 파일 삭제
            cout<<"삭제\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
    directoryPath = "/home/haena/DBDBDeep/IoTDB-lsm/src/test/compactionSSTable"; 
    try {
        // 디렉터리 내의 모든 파일 순회
        for (const auto& entry : filesystem::directory_iterator(directoryPath)) {
            filesystem::remove(entry.path()); // 파일 삭제
            cout<<"삭제\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void Workload::makeSSTable() {

    deleteAllSSTable(); //기존 파일 삭제

    cout<<"makeSSTable\n";

    int fileCounter=0;

    string filename="/home/haena/DBDBDeep/IoTDB-lsm/src/test/SSTable/NormalSStable";

    for(auto sstable:tree->Disk->normalSSTables){

        filename="/home/haena/DBDBDeep/IoTDB-lsm/src/test/SSTable/NormalSStable"+to_string(++fileCounter) + ".txt";

        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cerr << "Failed to open output file: ??" << filename << endl;
            return;
        }

        outputFile<<sstable->ss.begin()->first<<"\t"<<sstable->ss.end()->first<<"\n"; //처음키, 마지막키
        for (const auto& pair : sstable->ss) {
            outputFile << pair.first << "\t" << pair.second << "\n";
        }

        outputFile.close();
    }

    fileCounter=0;
    


    
    for(auto sstable:tree->Disk->delaySSTables){

        filename="/home/haena/DBDBDeep/IoTDB-lsm/src/test/SSTable/DelaySStable"+to_string(++fileCounter) + ".txt";

        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cerr << "Failed to open output file: " << filename << endl;
            return;
        }

        outputFile<<sstable->ss.begin()->first<<"\t"<<sstable->ss.end()->first<<"\n"; //처음키, 마지막키
        for (const auto& pair : sstable->ss) {
            outputFile << pair.first << "\t" << pair.second << "\n";
        }

        outputFile.close();
    }

}


void Workload::printDelayData(){

    int delayImmMemtableNum=0;
    int delayActiveMemtableNum=tree->activeDelayMemtable->mem.size();
    int normalImmMemtableNum=0;
    for(auto memtable : tree->immMemtableList){
        if(memtable->type=='DI') delayImmMemtableNum++;
        else normalImmMemtableNum++;
    }

    int delaySSTableNum= tree->Disk->delaySSTables.size();
    int delaySSTableSize=0;
    if(delaySSTableNum!= 0){
        delaySSTableSize= tree->Disk->delaySSTables.front()->ss.size();
    }

    int memory = delayImmMemtableNum*delaySSTableSize+delayActiveMemtableNum;
    int disk= delaySSTableNum*delaySSTableSize;
    int normal = normalImmMemtableNum * delaySSTableSize;
    // cout<<"delay delay data in Active Memtable : "<< delayActiveMemtableNum<<"\n";
    // cout<<"delay delay data in Imm Memtable : "<< delayImmMemtableNum*delaySSTableSize<<"\n";
    cout<<"delay data in Memory : "<< memory<<"\n";
    cout<<"delay data in Disk : "<<disk<<"\n";
    cout<<"normal data in Disk : "<<normal<<"\n";
    cout<<"delay rate : "<< disk/(disk+normal)<<"\n";
    cout<<"total delay: "<<memory+disk<<"\n";

    
}
