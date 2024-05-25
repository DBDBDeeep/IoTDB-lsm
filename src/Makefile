# 컴파일러 및 플래그 설정
CXX = g++
CXXFLAGS = -std=c++17 -lstdc++fs -I./src

# 타겟 정의
all: makeDataset testIoTDB makeWorkload

# makeDataset 타겟
makeDataset: makeDataset.o DataFactory.o DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o
	$(CXX) -o makeDataset makeDataset.o DataFactory.o DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o -lstdc++fs

# testIoTDB 타겟
testIoTDB: DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o DataFactory.o CompactionTest.o Workload.o testIoTDB.o
	$(CXX) -o testIoTDB DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o DataFactory.o CompactionTest.o Workload.o testIoTDB.o -lstdc++fs

# makeWorkload 타겟
makeWorkload: makeWorkload.o Workload.o DataFactory.o DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o
	$(CXX) -o makeWorkload makeWorkload.o Workload.o DataFactory.o DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o -lstdc++fs
# 개별 소스 파일을 오브젝트 파일로 컴파일하는 규칙
makeDataset.o: makeDataset.cpp
	$(CXX) $(CXXFLAGS) -c makeDataset.cpp -o makeDataset.o

makeWorkload.o: makeWorkload.cpp
	$(CXX) $(CXXFLAGS) -c makeWorkload.cpp -o makeWorkload.o

DataFactory.o: test/DataFactory.cpp
	$(CXX) $(CXXFLAGS) -c test/DataFactory.cpp -o DataFactory.o

DBManager.o: core/DBManager.cpp
	$(CXX) $(CXXFLAGS) -c core/DBManager.cpp -o DBManager.o

IMemtable.o: core/memtable/IMemtable.cpp
	$(CXX) $(CXXFLAGS) -c core/memtable/IMemtable.cpp -o IMemtable.o

DelayMemtable.o: core/memtable/DelayMemtable.cpp
	$(CXX) $(CXXFLAGS) -c core/memtable/DelayMemtable.cpp -o DelayMemtable.o

NormalMemtable.o: core/memtable/NormalMemtable.cpp
	$(CXX) $(CXXFLAGS) -c core/memtable/NormalMemtable.cpp -o NormalMemtable.o

MockDisk.o: disk/MockDisk.cpp
	$(CXX) $(CXXFLAGS) -c disk/MockDisk.cpp -o MockDisk.o

SSTable.o: disk/SSTable.cpp
	$(CXX) $(CXXFLAGS) -c disk/SSTable.cpp -o SSTable.o

CompactionTest.o: test/CompactionTest.cpp
	$(CXX) $(CXXFLAGS) -c test/CompactionTest.cpp -o CompactionTest.o

Workload.o: test/workload/Workload.cpp
	$(CXX) $(CXXFLAGS) -c test/workload/Workload.cpp -o Workload.o

testIoTDB.o: testIoTDB.cpp
	$(CXX) $(CXXFLAGS) -c testIoTDB.cpp -o testIoTDB.o

# 클린업 타겟
clean:
	rm -f makeDataset testIoTDB makeWorkload
	rm -f *.o

.PHONY: all clean
