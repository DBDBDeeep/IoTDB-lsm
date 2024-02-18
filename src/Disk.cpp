#include <map>
#include <list>
using namespace std;


struct SSTable{
    map<unsigned int, int> ss;
};

// 흉내 디스크
struct Disk{
public:
    list<SSTable> normal;
    list<SSTable> delay;
    int flushCount;
    int compactionCount; // ?
    bool compaction(); // 흉내만
    bool flush(Memtable& mem); // mem 클리어? 흉내만
};