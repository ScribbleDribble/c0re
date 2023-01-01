project(tests)
cmake_minimum_required(VERSION 3.10)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_string ../string.c test_string.cpp)
add_executable(test_vmm ../vmm.c ../pmm.c ../../drivers/vga.c ../string.c test_vmm.cpp)
target_link_libraries(test_string ${GTEST_LIBRARIES} pthread)
target_link_libraries(test_vmm ${GTEST_LIBRARIES} pthread)
