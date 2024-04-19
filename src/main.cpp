

#include "core/LSM.h"
#include "test/dataFactory.h"
#include <iostream>

using namespace std;

//LSM* tree;

// 테스트 리스트
void 기본테스트();
void 딜레이테스트();

int main(){

    dataFactory factory;

    /**parameter 설정
        o3데이터 없이 생성하고 싶으면 outOfOrderRatio = 0, numSegments = 0**/
    int n = 4000; // 데이터셋 크기
    double outOfOrderRatio = 0.1; // out of order 비율
    int numSegments = 10; // out of order 세그먼트 개수

    /**out of order 없는 data 테스트**/

//    factory.generateNormalDataset(n);
//    factory.NormalTest();

    /**out of order data 테스트**/
    factory.generateDelayedDataset(n,outOfOrderRatio,numSegments);
    factory.delayedTest();

    return 0;

    cout<<"[Read] key : "<<1200<<" value : "<<tree->readData(1200)<<"\n"; // normalSSTable read
    // Active에만 있는 data를 읽으려 할 때 불가! : -1
    cout<<"[Read] key : "<<2390<<" value : "<<tree->readData(2390)<<"\n";

    // 하나의 normal ImmMemtable range
    cout<<"\n[range] 하나의 normal ImmMemtable range : 2020 ~ 2030\n";
    map<uint64_t, int> result = tree->range(2020,2030);
    for(auto data: result){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }
    // 여러개의 normal ImmMemtable range
    cout<<"\n[range] 여러개의 normal ImmMemtable range : 2005 ~ 2015\n";
    map<uint64_t, int> result1 = tree->range(2005,2015);
    for(auto data: result1){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    // 한개의 SStalble range
    cout<<"\n[range] 한개의 SStalble range : 700 ~ 710\n";
    map<uint64_t, int> result2 = tree->range(700,710);
    for(auto data: result2){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    // 여러개의 SStalble range
    cout<<"\n[range] 여러개의 SStalble range : 330 ~ 340\n";
    map<uint64_t, int> result3 = tree->range(330,340);
    for(auto data: result3){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    // ImmTable과 SStable에 섞여있을때
    cout<<"\n[range] ImmTable과 SStable에 섞여있을때 : 1670 ~ 1680\n";
    map<uint64_t, int> result4 = tree->range(1670,1680);
    for(auto data: result4){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }


}


//void 기본테스트(){
//    tree = new LSM();
//    //normal test
//    for(int i=1;i<100;i++){
//        tree->insert(i, i*2);
//    }
//
//    tree->printActiveMemtable(true);
//    tree->printImmMemtable();
//    tree->disk->printSSTableList();
//
//    cout<<"\n\n\n\n========read/range test=========\n";
//
//    cout<<"[Read] key : "<<2009<<" value : "<<tree->readData(2009)<<"\n"; // normal ImmMemtable read
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
//    for (auto data: result3) {
//        cout << "key : " << data.first << " value : " << data.second << "\n";
//    }
//}
//
//void 기본테스트_ImmMemtable_SSTable_모두걸친_range(){
//    map<uint64_t, int> result4 = tree->range(1025,1035);
//    cout<<"\n[range] 1025 ~ 1035\n";
//    for(auto data: result4){
//        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
//    }
//}


void 딜레이테스트(){
    tree = new LSM();

    for(int i=100;i<200;i++){
        tree->insert(i, i*2);
    }
    for(int i=1;i<10;i++){
        tree->insert(i, i*2);
    }

    for(int i=400;i<700;i++){
        tree->insert(i, i*2);
    }
    for(int i=20;i<80;i++){
        tree->insert(i, i*2);
    }

    for(int i=700;i<800;i++){
        tree->insert(i, i*2);
    }

    for(int i=300;i<350;i++){
        tree->insert(i, i*2);
    }

    for(int i=1000;i<2000;i++){
        tree->insert(i, i*2);
    }
    for(int i=210;i<290;i++){
        tree->insert(i, i*2);
    }
    for(int i=850;i<990;i++){
        tree->insert(i, i*2);
    }
    for(int i=3000;i<3500;i++){
        tree->insert(i, i*2);
    }
    for(int i=2200;i<2900;i++){
        tree->insert(i, i*2);
    }

    tree->printActiveMemtable(false);
    tree->printImmMemtable();
    tree->disk->printSSTableList();

    cout<<"\n\n\n\n========read/range test=========\n";

    cout<<"key : "<<1200<<" value : "<<tree->readData(1200)<<"\n"; // normal ImmMemtable read
    cout<<"key : "<<1000<<" value : "<<tree->readData(1000)<<"\n"; // disk read

    cout<<"\n[range] 한개의 DelaySStalble range : 1~10\n";
    map<uint64_t, int> result = tree->range(1,10);
    for(auto data: result){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    cout<<"\n[range] 하나 DelaySStalble range : 330~340\n";
    map<uint64_t, int> result1 = tree->range(330,340);
    for(auto data: result1){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    cout<<"\n[range] 여러개 DelaySStalble range : 330~400\n";
    map<uint64_t, int> result2 = tree->range(330,400);
    for(auto data: result2){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    cout<<"\n[range] 여러개 immMemtable range : 2860~2865\n";
    map<uint64_t, int> result3 = tree->range(2860,2865);
    for(auto data: result3){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    delete tree;
}
//}
