include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(iotdb
        iotdb_test.cpp
        ../src/core/LSM.cpp
)

target_link_libraries(iotdb gtest gtest_main)