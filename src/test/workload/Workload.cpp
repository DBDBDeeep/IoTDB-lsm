#include "Workload.h"

using namespace std;

//파일에 있던 정보를 읽어와서 데이터셋 생성
vector<Record> Workload::readFile(const string& filePath) {
    vector<Record> dataset;
    ifstream file(filePath.c_str());
    if (!file.is_open()) {
        cerr << "ERR: 파일을 열 수 없습니다 " << filePath << endl;
        return dataset;
    }

    string line;
    int lineCount = 0; // 현재까지 읽은 줄 수를 추적하기 위한 변수

    cout<<filePath<<" 읽어오기 시작\n";
    while (getline(file, line)) {
        istringstream iss(line);
        string op;
        if (getline(iss, op, ',')) {
            Record record;
            record.op = op;
            if (op == "RANGE") {
                string startKeyStr, endKeyStr;
                if (getline(iss, startKeyStr, ' ') && getline(iss, endKeyStr)) {
                    record.start_key = stoull(startKeyStr);
                    record.end_key = stoull(endKeyStr);
                } else {
                    cerr << "ERR: 잘못된 형식의 RANGE 레코드입니다: " << line << endl;
                    continue;
                }
            } else {
                string keyStr;
                if (getline(iss, keyStr)) {
                    record.key = stoull(keyStr);
                } else {
                    cerr << "ERR: 잘못된 형식의 " << op << " 레코드입니다: " << line << endl;
                    continue;
                }
            }
            dataset.push_back(record);
        } else {
            cerr << "ERR: 잘못된 형식의 레코드입니다: " << line << endl;
        }
        /**진행률 출력 (전체 크기 기준으로 출력하기 어려우므로 임의의 기준으로 출력)*/
        ++lineCount;
        if (lineCount % 5000000 == 0){
            cout<<lineCount<<"개 읽음\n";
        }
    }
    cout<<filePath<<" 읽어오기 끝\n";

    file.close();
    return dataset;
}
void Workload::executeWorkload(vector<Record>& dataset){

    cout<<"workload 실행 시작\n";
    for(int i=0; i<dataset.size(); i++){
        //cout<<dataset[i].op<<" "<<dataset[i].key<<"\n";
        if (dataset[i].op == "READ") {
            tree->readData(dataset[i].key);
        } else if (dataset[i].op == "RANGE") {
            tree->range(dataset[i].start_key, dataset[i].end_key);
        } else if(dataset[i].op == "INSERT"){
            tree->insert(dataset[i].key, dataset[i].key*2);
        }else{
            cerr << "ERR: 잘못된 형식의 레코드입니다: " << dataset[i].op << endl;
        }
        /**진행률 출력*/
        if (i != 0 && i % (dataset.size() / 100) == 0) {
            VECTOR_LOG_PROGRESS(i, dataset);
        }
    }

    cout<<"\nworkload 실행 끝\n";


}

DBManager* Workload::getTree() {
    return tree;
}
void Workload::cleanup() {
    delete tree;
    tree = nullptr;
}