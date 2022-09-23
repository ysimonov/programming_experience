cmake_minimum_required(VERSION 3.5)
 
project(FizzBuzz)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(FizzBuzz FizzBuzz.cpp)
target_link_libraries(FizzBuzz ${GTEST_LIBRARIES} pthread)