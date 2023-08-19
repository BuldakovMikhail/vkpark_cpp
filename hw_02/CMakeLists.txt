cmake_minimum_required(VERSION 3.10)
project(hw_02)

set(CMAKE_CXX_STANDARD 17)
include_directories(./inc)

add_executable(hw_02 ./src/expression_handling.cpp
        ./src/operations_parcing.cpp
        ./src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
