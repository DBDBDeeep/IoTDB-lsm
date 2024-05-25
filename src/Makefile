# 컴파일러 및 플래그 설정
CXX = g++
CXXFLAGS = -std=c++17 -I./src

makeDataset : makeDataset.o DataFactory.o
	$(CXX) $(CXXFLAGS) -c src/makeDataset.cpp -o makeDataset.o
	$(CXX) $(CXXFLAGS) -c src/test/DataFactory.cpp -o DataFactory.o
	$(CXX) -o makeDataset makeDataset.o DataFactory.o
	
makeWorkload: Workload.o DataFactory.o makeWorkload.o
	$(CXX) $(CXXFLAGS) -c src/makeDataset.cpp -o makeDataset.o
	$(CXX) $(CXXFLAGS) -c src/test/DataFactory.cpp -o DataFactory.o
	$(CXX) $(CXXFLAGS) -c src/test/makeWorkload.cpp -o makeWorkload.o
	$(CXX) -o makeDataset makeDataset.o DataFactory.o makeWorkload.o
	
testIoTDB: DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o DataFactory.o CompactionTest.o Workload.o testIoTDB.o
$(CXX) -o testIoTDB DBManager.o IMemtable.o DelayMemtable.o NormalMemtable.o MockDisk.o SSTable.o DataFactory.o CompactionTest.o Workload.o testIoTDB.o
	
DBManager.o: src/core/DBManager.cpp
$(CXX) $(CXXFLAGS) -c src/core/DBManager.cpp -o DBManager.o

IMemtable.o: src/core/memtable/IMemtable.cpp
$(CXX) $(CXXFLAGS) -c src/core/memtable/IMemtable.cpp -o IMemtable.o

DelayMemtable.o: src/core/memtable/DelayMemtable.cpp
$(CXX) $(CXXFLAGS) -c src/core/memtable/DelayMemtable.cpp -o DelayMemtable.o

NormalMemtable.o: src/core/memtable/NormalMemtable.cpp
$(CXX) $(CXXFLAGS) -c src/core/memtable/NormalMemtable.cpp -o NormalMemtable.o

MockDisk.o: src/disk/MockDisk.cpp
$(CXX) $(CXXFLAGS) -c src/disk/MockDisk.cpp -o MockDisk.o

SSTable.o: src/disk/SSTable.cpp
$(CXX) $(CXXFLAGS) -c src/disk/SSTable.cpp -o SSTable.o

DataFactory.o: src/test/DataFactory.cpp
$(CXX) $(CXXFLAGS) -c src/test/DataFactory.cpp -o DataFactory.o

CompactionTest.o: src/test/CompactionTest.cpp
$(CXX) $(CXXFLAGS) -c src/test/CompactionTest.cpp -o CompactionTest.o

Workload.o: src/test/workload/Workload.cpp
$(CXX) $(CXXFLAGS) -c src/test/workload/Workload.cpp -o Workload.o

testIoTDB.o: src/testIoTDB.cpp
$(CXX) $(CXXFLAGS) -c src/testIoTDB.cpp -o testIoTDB.o
	
	
clean:
	rm -f makeDataset
	rm -f makeWorkload
	rm -f testIoTDB
	rm -f *.o