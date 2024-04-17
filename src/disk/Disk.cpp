#include "Disk.h"


bool Disk::compaction() {
    //Todo: 구현
    return true;
}

int Disk::read(uint64_t key) {

    //normal SStale 뒤지기
    for (auto ss : normalSSTables) {
                // 맵에서 키 검색
        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    //없으면 delay SStale 뒤지기
    for (auto ss : delaySSTables) {
        // 맵에서 키 검색
        auto it = ss->ss.find(key);
        if (it != ss->ss.end()) {
            return it->second;  // 키를 찾았으면 값 반환
        }
    }

    //없어요
    return -1;
}

map<uint64_t, int> Disk::range(uint64_t start, uint64_t end) {
    }

    }

}
    }

