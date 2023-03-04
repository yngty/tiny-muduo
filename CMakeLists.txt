cmake_minimum_required(VERSION 3.14)
project(tiny-muduo)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBMUDUO_PROJECT_DIR   ${PROJECT_SOURCE_DIR})

add_subdirectory(src)

install(
    DIRECTORY "include/"
    DESTINATION "include"
)