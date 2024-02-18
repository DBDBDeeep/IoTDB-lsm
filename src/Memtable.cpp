#include <map>
using namespace std;

enum State{
    ACTIVE, IMM
};

struct Memtable{
public:
    State state;
    virtual bool isFull();
    virtual bool setState(State state);
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

    bool isFull() override{
        size_t currentSize = normalMem.size() * (sizeof(unsigned int) + sizeof(int));
        return currentSize >= LIMIT_SIZE;
    }
    bool setState(State state) override{
        this->state = state;
        return true;
    }
};

struct DelayMemtable : public Memtable{
public:
    DelayMemtable(){
        state = ACTIVE;
    }

    const size_t LIMIT_SIZE = 40;
    map<unsigned int, int> delayMem;

    bool isFull() override{
        size_t currentSize = delayMem.size() * (sizeof(unsigned int) + sizeof(int));
        return currentSize >= LIMIT_SIZE;
    }
    bool setState(State state) override{
        this->state = state;
        return true;
    }
};