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
    int LIMIT_SIZE = 80;
    map<unsigned int, int> normalMem;
    static int access; // IMM만 써
    bool isFull(){
        size_t currentSize = normalMem.size() * (sizeof(unsigned int) + sizeof(int));
        return currentSize >= LIMIT_SIZE;
    }
};

struct DelayMemtable : public Memtable{
public:
    int LIMIT_SIZE = 40;
    map<unsigned int, int> delayMem;
    bool isFull(){
        size_t currentSize = delayMem.size() * (sizeof(unsigned int) + sizeof(int));
        return currentSize >= LIMIT_SIZE;
    }
};