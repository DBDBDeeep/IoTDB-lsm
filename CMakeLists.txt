cmake_minimum_required(VERSION 3.27)
project(IoTDB_lsm)

set(CMAKE_CXX_STANDARD 17)

add_executable(IoTDB_lsm
        src/core/LSM.cpp
        tests/iotdb_test.cpp)

add_subdirectory(tests)
add_subdirectory(tests/lib/googletest-1.14.0)