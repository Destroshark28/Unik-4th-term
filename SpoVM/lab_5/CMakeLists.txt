cmake_minimum_required(VERSION 3.14)

project(lab_5)

set(CMAKE_CXX_STANDARD 14)

include(asyncio/CMakeLists.txt)

add_executable(lab_5 main.cpp)

target_link_libraries(lab_5 pthread)
target_link_libraries(lab_5 ${CMAKE_DL_LIBS})

