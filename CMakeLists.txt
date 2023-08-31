cmake_minimum_required(VERSION 3.23)
project(Tiles)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_executable(Tiles main.cpp)
