file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_definitions(-D__BUILDING_OPENSTL_DATA)

add_library(Data SHARED ${ALL_SOURCE})
add_library(OpenSTL.Data ALIAS Data)

target_link_libraries(Data OpenSTL.Math)

set_target_properties(Data PROPERTIES OUTPUT_NAME openstl-data)


add_subdirectory(Tests)