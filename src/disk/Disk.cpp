#include <map>
#include <list>
using namespace std;


struct SSTable{
    map<unsigned int, int> ss;
};

// TODO : normalSSTables
// 흉내 디스크
struct Disk{
private:
    long compactionCount;
    long flushCount;
    void doCompaction(){ compactionCount++; }
    void doCompaction(int cnt){ compactionCount += cnt; }
    void doFlush(){ flushCount++; }
    void doFlush(int cnt){ flushCount += cnt; }
public:
    list<SSTable> normalSSTables;
    list<SSTable> delaySSTables;
    long readCount;

    Disk(){
        flushCount = 0;
        compactionCount = 0;
        readCount = 0;
    }

    bool compaction() { // 흉내만
        doCompaction();
    }

    void read(unsigned int key){
        // TODO : SSTable에서 찾기!
    }

    void range(unsigned int start, unsigned int end){
        // TODO : SSTable에서 찾기!
    }

    bool flush(IMemtable& mem) {// mem 클리어? 흉내만
        doFlush();
    }

};