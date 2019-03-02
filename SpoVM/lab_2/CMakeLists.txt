cmake_minimum_required(VERSION 3.13)

project(lab_2)
set(CMAKE_CXX_STANDARD 14)
add_executable(lab_2 Parent.cpp)

project(lab_2_child)
set(CMAKE_CXX_STANDARD 14)
add_executable(lab_2_child Child.cpp)