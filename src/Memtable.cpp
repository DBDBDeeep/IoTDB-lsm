#include <map>
using namespace std;

enum State{
    ACTIVE, IMM
};

struct Memtable{
public:
    State state;
};

struct NormalMemtable : public Memtable{
public:
    NormalMemtable(){
        state = ACTIVE;
        access = 0;
    }

    const size_t LIMIT_SIZE = 80;
    map<unsigned int, int> normalMem;
    int access; // IMM만 써

        size_t currentSize = normalMem.size() * (sizeof(unsigned int) + sizeof(int));
        return currentSize >= LIMIT_SIZE;
    }
};

struct DelayMemtable : public Memtable{
public:
    DelayMemtable(){
        state = ACTIVE;
    }

    const size_t LIMIT_SIZE = 40;
    map<unsigned int, int> delayMem;
    bool isFull(){
        size_t currentSize = delayMem.size() * (sizeof(unsigned int) + sizeof(int));
        return currentSize >= LIMIT_SIZE;
    }
};