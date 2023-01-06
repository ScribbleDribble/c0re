project(tests)
cmake_minimum_required(VERSION 3.10)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_vga ../vga.c ../../kernel/string.c test_vga.cpp)
target_link_libraries(test_vga ${GTEST_LIBRARIES} pthread)
