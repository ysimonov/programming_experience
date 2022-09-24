cmake_minimum_required(VERSION 3.5)
 
project(Checkout)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(Checkout Checkout.cpp CheckoutTest.cpp)
target_link_libraries(Checkout ${GTEST_LIBRARIES} pthread)