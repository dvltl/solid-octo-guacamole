cmake_minimum_required(VERSION 3.5)
project(01_allocator)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O1")


include_directories("../thirdparty")

set(TEST_FILES ../thirdparty/gtest/gtest-all.cc ../thirdparty/gtest/gtest_main.cc)
set(SRC allocator.cpp allocator_error.cpp allocator_pointer.cpp allocator_test.cpp)
set(HDR allocator.h allocator_error.h allocator_pointer.h)

add_executable(01_allocator ${SRC} ${TEST_FILES})
target_link_libraries(01_allocator pthread)


#all: tests.done

#allocator_test: $(SRC) $(HDR)
#g++ -O1 -g -std=c++11 -o allocator_test $(SRC) -I../thirdparty $(TEST_FILES) -lpthread

#tests.done: allocator_test
#./allocator_test
#touch tests.done
