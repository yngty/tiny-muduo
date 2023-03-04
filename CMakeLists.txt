cmake_minimum_required(VERSION 3.14)
project(tiny-muduo)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBMUDUO_PROJECT_DIR   ${PROJECT_SOURCE_DIR})

include(etc/clang_format.cmake)
include(etc/clang_tidy.cmake)
include(etc/cppcheck.cmake)

add_subdirectory(src)
add_subdirectory(demo)
install(
    DIRECTORY "include/"
    DESTINATION "include"
)