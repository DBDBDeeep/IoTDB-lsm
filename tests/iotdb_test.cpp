//#include <gtest/gtest.h>
//#include "../src/core/LSM.h"
////#include "lib/googletest-1.14.0/googletest/include/gtest/gtest.h"
//
//using namespace std;
//
//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
//
//TEST(yaho1,test){
////given
//    LSM* lsm = new LSM();
//
//    //when
//    for(int i=1;i<10;i++) {
//        lsm->insert(i, i * 111);
//    }
//
//    //then
//    int start = 2, end = 5;
//
//    map results = lsm->range(start, end);
//    EXPECT_EQ(end-start+1, results.size());
//
//    int compare = start;
//    for (const auto& pair : results) {
//        cout << pair.first << "==" << compare <<"\n";
//        EXPECT_EQ(pair.first, compare);
//        EXPECT_EQ(pair.second, compare*111);
//        compare++;
//    }
//}