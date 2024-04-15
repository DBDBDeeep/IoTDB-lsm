#ifndef IOTDB_LSM_COMPACTIONCONTROLLER_H
#define IOTDB_LSM_COMPACTIONCONTROLLER_H


class CompactionController {
private:
    void findCompactionMem();
    void checkTimeOut();
};


#endif //IOTDB_LSM_COMPACTIONCONTROLLER_H
