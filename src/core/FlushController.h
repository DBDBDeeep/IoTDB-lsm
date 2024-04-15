#ifndef IOTDB_LSM_FLUSHCONTROLLER_H
#define IOTDB_LSM_FLUSHCONTROLLER_H


using namespace std;

class FlushController {
private:
    void findFlushMem();
    void checkTimeout();
};

#endif //IOTDB_LSM_FLUSHCONTROLLER_H