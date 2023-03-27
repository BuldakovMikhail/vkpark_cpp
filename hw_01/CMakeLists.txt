cmake_minimum_required(VERSION 3.0.0)
project(hw_01 VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 17)
include_directories(./inc)
enable_testing()

add_executable(hw_01 ./src/main.cpp 
            ./src/string_processing.cpp
            ./src/vector_operations.cpp
            ./src/files_handlers.cpp
            ./src/file_handlers.cpp src/arguments_handler.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
