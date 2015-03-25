cmake_minimum_required(VERSION 2.8)
project(OpenSTL)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(OpenSTL)
add_subdirectory(Tests)
add_subdirectory(Demo)