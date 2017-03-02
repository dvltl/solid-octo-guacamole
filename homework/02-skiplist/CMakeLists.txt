cmake_minimum_required(VERSION 3.5)
project(02_skiplist)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O1")# -L/lib/x86_64-linux-gnu")


include_directories("../thirdparty")
include_directories("./include")
set(INCL ./include/skiplist)

set(TEST_FILES ../thirdparty/gtest/gtest-all.cc ../thirdparty/gtest/gtest_main.cc)
set(HDR ${INCL}/node.h ${INCL}/iterator.h ${INCL}/skiplist.h)
set(TEST test/skiplist_test.cpp)

add_executable(02_skiplist ${HDR} ${TEST_FILES} ${TEST})
