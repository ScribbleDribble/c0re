project(tests)
cmake_minimum_required(VERSION 3.10)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_string ../string.c test_string.cpp)
add_executable(test_mem_manager ../mem_manager.c test_mem_manager.cpp)
target_link_libraries(test_string ${GTEST_LIBRARIES} pthread)
target_link_libraries(test_mem_manager ${GTEST_LIBRARIES} pthread)
