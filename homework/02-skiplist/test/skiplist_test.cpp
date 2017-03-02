#include "gtest/gtest.h"
#include <string>
#include <skiplist/skiplist.h>

using namespace std;

SkipList<int, string, 8> sk;

TEST(SkipListTest, Empty) {
  ASSERT_EQ(nullptr, sk.Get(100));
  ASSERT_EQ(sk.cend(), sk.cbegin())  << "Begin iterator fails";
  ASSERT_EQ(sk.cend(), sk.cfind(10)) << "Find iterator fails";
}

TEST(SkipListTest, SimplePut) {
  SkipList<int, string, 8> sk;

  const std::string *pOld = sk.Put(10, "test");
  ASSERT_EQ(nullptr, pOld);

  pOld = sk[10];
  ASSERT_NE(nullptr, pOld)         << "Value found";
  ASSERT_EQ(string("test"), *pOld) << "Value is correct";

  pOld = sk.Get(10);
  ASSERT_NE(nullptr, pOld)         << "Value found";
  ASSERT_EQ(string("test"), *pOld) << "Value is correct";

  Iterator<int,std::string> it = sk.cbegin();
  ASSERT_NE(sk.cend(), it)              << "Iterator is not empty";
  ASSERT_EQ(10, it.key())               << "Iterator key is correct";
  ASSERT_EQ(string("test"), it.value()) << "Iterator value is correct";
  ASSERT_EQ(string("test"), *it)        << "Iterator value is correct";
}

// new tests

TEST(SkipListTest, PutInEmpty) {
    SkipList<int, float, 4> sk;
    float * buf = sk.Put(0, 2.0);
    ASSERT_EQ(nullptr, buf);
    delete buf;
}

TEST(SkipListTest, PutIfAbsentInNotEmpty) {
    SkipList<int, float, 4> sk;
    float * buf = sk.Put(0, 2.0);
    ASSERT_EQ(nullptr, buf);
    delete buf;
    buf = sk.PutIfAbsent(0, 1.1);
    ASSERT_NE(nullptr, buf);
    ASSERT_EQ(2.0, *buf);
    delete buf;
    buf = sk.PutIfAbsent(0, 1.1); //to check that list wasn't changed
    ASSERT_NE(nullptr, buf);
    ASSERT_EQ(2.0, *buf);
    delete buf;
}

TEST(SkipListTest, GetEmpty) {
    SkipList<int, float, 4> sk;
    float * buf = sk.Get(0);
    ASSERT_EQ(nullptr, buf);
}

TEST(SkipListTest, GetNotEmpty) {
    SkipList<int, float, 4> sk;
    float * buf = sk.Put(0, 2.0);
    ASSERT_EQ(nullptr, buf);
    delete buf;
    buf = sk.Get(0);
    ASSERT_NE(nullptr, buf);
    ASSERT_EQ(2.0, *buf);
    delete buf;
}

TEST(SkipListTest, PutExistingInNotEmpty) {
    SkipList<int, float, 4> sk;
    float * buf = sk.Put(0, 2.0);
    ASSERT_EQ(nullptr, buf);
    delete buf;
    buf = sk.Put(0, 1.1);
    ASSERT_NE(nullptr, buf);
    ASSERT_EQ(2.0, *buf);
    delete buf;
    buf = sk.Get(0);
    ASSERT_NE(nullptr, buf);
    ASSERT_EQ(*buf, 1.1);
    delete buf;
}
