cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
project(o3d VERSION 0.1.0)


add_executable(o3d open3d_cpp.cpp)

find_package(Open3D REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

target_include_directories(o3d PUBLIC ${Open3D_INCLUDE_DIRS})

target_link_libraries(o3d PRIVATE Eigen3::Eigen ${Open3D_LIBRARIES})
