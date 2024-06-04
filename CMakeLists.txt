cmake_minimum_required(VERSION 3.26.4)
project(IoTDB_lsm)

set(CMAKE_CXX_STANDARD 17)

add_executable(IoTDB_lsm
        #src/main.cpp
        src/core/DBManager.cpp
        src/core/memtable/IMemtable.cpp
        src/core/memtable/DelayMemtable.cpp
        src/core/memtable/NormalMemtable.cpp
        src/disk/MockDisk.cpp
        src/disk/SSTable.cpp
        src/test/DataFactory.cpp
        src/test/CompactionTest.cpp
        src/test/workload/Workload.cpp
        src/core/FlushController.h
        #        src/makeDataset.cpp
                src/testIoTDB.cpp
#        src/makeWorkload.cpp
)


include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/core
        ${PROJECT_SOURCE_DIR}/src/core/memtable
        ${PROJECT_SOURCE_DIR}/src/disk
)

