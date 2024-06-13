
#include "test/DataFactory.h"

int main(int argc, char* argv[]) {
    
    DataFactory factory;

    string dataSetName = argv[1]; //ex) "dataA";
    int initDataCount = atoi(argv[2]); // 데이터셋 크기
    double outOfOrderRatio = stod(argv[3]); // out of order 비율

    factory.generateO3Dataset(dataSetName, initDataCount, outOfOrderRatio);

}