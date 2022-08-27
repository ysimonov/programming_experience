cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(sor_filter)

find_package(PCL 1.2 REQUIRED)

# Set target C++ standard and required compiler features
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(sor_filter sor_filter.cpp)
target_link_libraries(sor_filter ${PCL_LIBRARIES})
