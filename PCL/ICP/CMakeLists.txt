cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(icp)

# Set target C++ standard and required compiler features
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PCL 1.5 REQUIRED)

if (PCL_FOUND)

# [C/C++]>[General]>[Additional Include Directories]
include_directories(${PCL_INCLUDE_DIRS})

# [Linker]>[General]>[Additional Library Directories]
link_directories(${PCL_LIBRARY_DIRS})

# [C/C++]>[Preprocessor]>[Preprocessor Definitions]
add_definitions(${PCL_DEFINITIONS})

# executable
add_executable(icp icp.cpp)

# [Linker]>[Input]>[Additional Dependencies]
target_link_libraries(icp ${PCL_LIBRARIES})

message(STATUS ${PCL_LIBRARIES})

endif()

