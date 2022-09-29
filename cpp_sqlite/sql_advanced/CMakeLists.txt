cmake_minimum_required(VERSION 3.0.0)

set(SQL_PROJECT_NAME SqlExample)

set(SQL_PROJECT_DIR src)

set(SQL_SOURCE ${SQL_PROJECT_DIR}/SqlExample.cpp)

set(SQL_LIBRARIES_DIR libs)

set (SQL_LIBRARIES
    # list libraries
)

set (INCLUDE_DIRECTORY include)

project(${SQL_PROJECT_NAME})

add_executable(${SQL_PROJECT_NAME} ${SQL_LIBRARIES_DIR}/sqlite3.c ${SQL_SOURCE})

# foreach (LIBRARY $(SQL_LIBRARIES))
#     add_subdirectory("${SQL_LIBRARIES_DIR}/${LIBRARY}")
# endforeach(LIBRARY)

target_link_libraries(${SQL_PROJECT_NAME} ${SQL_LIBRARIES})
target_include_directories(SqlExample PRIVATE ${INCLUDE_DIRECTORY})