cmake_minimum_required(VERSION 3.5)
project(02_skiplist)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O1")


include_directories("../thirdparty")
include_directories("./include")
set(INCL ./include/skiplist)

set(SRC main.cpp)
set(HDR ${INCL}/node.h ${INCL}/iterator.h ${INCL}/skiplist.h)

add_executable(02_skiplist ${SRC} ${HDR})
