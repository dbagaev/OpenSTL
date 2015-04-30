file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(Math SHARED ${ALL_SOURCE})
add_library(OpenSTL.Math ALIAS Math)

set_target_properties(Math PROPERTIES OUTPUT_NAME openstl-math)





