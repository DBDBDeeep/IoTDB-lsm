

#include "core/LSM.h"
#include <iostream>



using namespace std;


int main(){

    LSM* tree = new LSM();

    //normal test
    for(int i=1;i<1750;i++){
        tree->insert(i, i*2);
    }

    tree->printActiveMemtable();
    tree->printImmMemtable();
    tree->disk->printSSTableList();

    cout<<"\n\n\n\n========read/range test=========\n";

    cout<<"[Read] key : "<<1200<<" value : "<<tree->readData(1200)<<"\n"; // normal ImmMemtable read
    cout<<"[Read] key : "<<1000<<" value : "<<tree->readData(1000)<<"\n"; // disk read

    // 하나의 normal ImmMemtable range
    map<uint64_t, int> result = tree->range(1400,1410);
    cout<<"\n[range] 1400 ~ 1410\n";
    for(auto data: result){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }
    // 여러개의 normal ImmMemtable range
    map<uint64_t, int> result1 = tree->range(1365,1375);
    cout<<"\n[range] 1365 ~ 1375\n";
    for(auto data: result1){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    // 한개의 SStalble range
    map<uint64_t, int> result2 = tree->range(10,20);
    cout<<"\n[range] 10 ~ 20\n";
    for(auto data: result2){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }
//
//    // 여러개의 SStalble range
    map<uint64_t, int> result3 = tree->range(340,350);
    cout<<"\n[range] 340 ~ 350\n";
    for(auto data: result3){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }

    // ImmTable과 SStable에 섞여있을때
    map<uint64_t, int> result4 = tree->range(1025,1035);
    cout<<"\n[range] 1025 ~ 1035\n";
    for(auto data: result4){
        cout<<"key : "<<data.first<<" value : "<<data.second<<"\n";
    }




    /*delay test
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

    tree->printActiveMemtable();
    tree->printImmMemtable();
    tree->disk->printSSTableList();

    cout<<"\n\n\n\n========read/range test=========\n";

    cout<<"key : "<<1200<<" value : "<<tree->readData(1200)<<"\n"; // normal ImmMemtable read
    cout<<"key : "<<1000<<" value : "<<tree->readData(1000)<<"\n"; // disk read
*/
    return 0;

}


