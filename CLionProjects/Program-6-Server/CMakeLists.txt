cmake_minimum_required(VERSION 3.4)
project(Program_6_Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp ConnectionHandler.cpp ConnectionHandler.h)
add_executable(Program_6_Server ${SOURCE_FILES})