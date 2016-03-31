cmake_minimum_required(VERSION 3.4)
project(Program_6_Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Client.cpp Client.h)
add_executable(Program_6_Client ${SOURCE_FILES})