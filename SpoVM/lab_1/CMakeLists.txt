cmake_minimum_required(VERSION 3.13)
project(lab_1 C)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_C_STANDARD 11)

add_executable(lab_1 main.c)
target_link_libraries(lab_1 ${CURSES_LIBRARIES})