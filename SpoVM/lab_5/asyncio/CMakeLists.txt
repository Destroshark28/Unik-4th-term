cmake_minimum_required(VERSION 3.14)

project(AsyncIO VERSION 1.0.0 DESCRIPTION "Async IO")
set(CMAKE_CXX_STANDARD 14)
add_library(${PROJECT_NAME} SHARED
        asyncio/AsyncReadWrite.h
        asyncio/AsyncReadWrite.h
        )
target_compile_options(${PROJECT_NAME} PUBLIC "-pie")
target_link_libraries(${PROJECT_NAME} "-pie -Wl,-E")
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
