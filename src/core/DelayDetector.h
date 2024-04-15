//
// Created by 강지윤 on 4/12/24.
//

#ifndef IOTDB_LSM_DELAYDETECTOR_H
#define IOTDB_LSM_DELAYDETECTOR_H

#include "../storage/Memtable.cpp"

using namespace std;

class DelayDetector {
public:
    long detect(IMemtable& memtable);
};


#endif //IOTDB_LSM_DELAYDETECTOR_H
