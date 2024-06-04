//
// Created by 강지윤 on 6/4/24.
//

#ifndef IOTDB_LSM_FLUSHCONTROLLER_H
#define IOTDB_LSM_FLUSHCONTROLLER_H

#include <condition_variable>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace std;


class FlushController {
public:
    FlushController(MockDisk* mockDisk, list<IMemtable*>& immMemtableList) : disk(mockDisk), flushQueue(immMemtableList){}
    // 시작 메소드
    void start(Type t) {
        std::lock_guard<std::mutex> lock(m_mutex);
        workers.emplace_back(&FlushController::run, this, t);
    }

    ~FlushController() { }
private:
    vector<thread> workers;
    std::condition_variable condition;
    list<IMemtable*>& flushQueue;
    std::mutex flushQueueMutex;
    mutex m_mutex;
    MockDisk* disk;

    void run(Type t){
        doFlush(t);

        // 스레드 종료를 위한 clean up
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = std::find_if(workers.begin(), workers.end(), [](std::thread &worker) {
                return worker.get_id() == std::this_thread::get_id();
            });
            if (it != workers.end()) {
                if (it->joinable()) {
                    it->detach();  // 스레드를 백그라운드로 돌려서 종료 대기
                }
                workers.erase(it);  // 벡터에서 제거
            }
            if (workers.empty()) {
                condition.notify_all();
            }
        }

    }

    void doFlush(Type t) {
        while (true) {
            cout<<t<<"doFlush\n";
            IMemtable *memtable = nullptr;
            // flushQueue 락
            {
                std::unique_lock<std::mutex> lock(flushQueueMutex);
                if (!flushQueue.empty()) {
                    memtable = flushQueue.front();
                    flushQueue.pop_front();
                } else {
                    break;
                }
            }

            if (memtable != nullptr) {
                if (disk->flush(memtable, t)) {
                    delete memtable;
                }
            }
        }
    }
};




#endif //IOTDB_LSM_FLUSHCONTROLLER_H
