#include "ActiveMemtableController.h"


vector<NormalMemtable*> activeNormalMemtableList;
vector<DelayMemtable*> activeDelayMemtableList;

void ActiveMemtableController::insert(unsigned int key, int value) {

}

bool ActiveMemtableController::isDelayData(unsigned int key) {

}