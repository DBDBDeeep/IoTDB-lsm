#include "ImmutableMemtableController.h"

vector<NormalMemtable*> normalMemtableList_M1;
vector<DelayMemtable*> delayMemtableList_M1;
vector<NormalMemtable*> normalMemtableList_M2;
vector<DelayMemtable*> delayMemtableList_M2;
queue<IMemtable*> compactionQueue;
queue<IMemtable*> flushQueue;
CompactProcessor compactProcessor;

map<unsigned int, int> ImmutableMemtableController::range(unsigned int start, unsigned int end) {

}

void ImmutableMemtableController::putMemtableToQueue(IMemtable &) {

}

int ImmutableMemtableController::read(unsigned int key) {

}

void ImmutableMemtableController::compaction() {
    // TODO : compactProcessor.compaction();
}

void ImmutableMemtableController::transformM1toM2(IMemtable &) {

}