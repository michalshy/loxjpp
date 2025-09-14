cmake_minimum_required(VERSION 3.10)

project(loxjpp VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES src/*.cpp src/*.hpp)
add_executable(loxjpp ${SRC_FILES})