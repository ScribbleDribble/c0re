project(tests)
cmake_minimum_required(VERSION 3.10)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_priority_queue.out ../priority_queue.c test_priority_queue.cpp)
target_link_libraries(test_priority_queue.out ${GTEST_LIBRARIES} pthread)
