#include <map>
#include <list>
using namespace std;


struct SSTable{
    map<unsigned int, int> ss;
};

// 흉내 디스크
struct Disk{
private:
    int compactionCount;
    int flushCount;
    void doCompaction(){ compactionCount++; }
    void doCompaction(int cnt){ compactionCount += cnt; }
    void doFlush(){ flushCount++; }
    void doFlush(int cnt){ flushCount += cnt; }
public:
    list<SSTable> normal;
    list<SSTable> delay;

    Disk(){
        flushCount = 0;
        compactionCount = 0;
    }

    bool compaction() { // 흉내만
        doCompaction();
    }
    bool flush(Memtable& mem) {// mem 클리어? 흉내만
        doFlush();
    }

};