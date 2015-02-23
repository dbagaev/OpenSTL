file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(Python SHARED ${ALL_SOURCE})
add_library(OpenSTL.Python ALIAS Python)
target_link_libraries(Python OpenSTL.Data)

set_target_properties(Python PROPERTIES OUTPUT_NAME py-openstl)
