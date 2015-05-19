file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

find_package(Boost REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${Boost_INCLUDE_DIR} ${Python_INCLUDE_DIR})

link_directories(${LINK_DIRECTORIES} ${Boost_LIB_DIR} ${Python_LIB_DIR})

add_executable(Demo ${ALL_SOURCE})

target_link_libraries(Demo OpenSTL.Data)
