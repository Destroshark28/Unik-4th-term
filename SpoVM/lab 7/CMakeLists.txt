cmake_minimum_required(VERSION 3.14)
project(lab_7)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_7 filesystem/impl/disk.cpp filesystem/impl/fs.cpp main.cpp filesystem/disk.h filesystem/fs.h)