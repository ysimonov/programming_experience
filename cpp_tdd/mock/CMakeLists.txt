cmake_minimum_required(VERSION 3.14)
 
project(GMockTest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(
    GMockTest 
    GMockTest.cpp
)

target_link_libraries(
    GMockTest 
    GTest::gtest_main
    gmock   # without this linking option, won't compile
)

include(GoogleTest)
gtest_discover_tests(GMockTest)