#include <map>
using namespace std;

enum State{
    ACTIVE, IMM
};

struct Memtable{
public:
    static State state;

};

struct NormalMemtable : public Memtable{
public:
    map<unsigned int, int> normalMem;
    static int access; // IMM만 써
};

struct DelayMemtable : public Memtable{
public:
    map<unsigned int, int> delayMem;


};