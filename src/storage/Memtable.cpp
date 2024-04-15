#include <map>
using namespace std;

enum State{
    ACTIVE, IMM
};

struct IMemtable{
public:
    State state;
    unsigned int startKey;
    unsigned int lastKey;
    virtual bool isFull();
    virtual void put(unsigned int key, int value);
    virtual bool setState(State state);
    virtual bool setStartKey(unsigned int key);
    virtual bool setLastKey();
};

struct NormalMemtable : public IMemtable{
public:
    NormalMemtable(){
        state = ACTIVE;
        access = 0; startKey = 0; lastKey = -1;
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
    void put(unsigned int key, int value) override{
        normalMem.emplace(key, value);
    }
    bool setStartKey(unsigned int key){
        startKey = key;
    }
    bool setLastKey(){
        lastKey = normalMem.end()->first;
    }
};

struct DelayMemtable : public IMemtable{
public:
    DelayMemtable(){
        state = ACTIVE;
        startKey = 0; lastKey = -1;
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
    void put(unsigned int key, int value) override{
        delayMem.emplace(key, value);
    }
    bool setStartKey(unsigned int key){
        startKey = key;
    }
    bool setLastKey(){
        lastKey = delayMem.end()->first;
    }
};