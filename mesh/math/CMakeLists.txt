file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(Math STATIC ${ALL_SOURCE})
add_library(mesh.Math ALIAS Math)

set_target_properties(Math PROPERTIES OUTPUT_NAME mesh-math)

if ( BUILD_TESTING )
    add_subdirectory(Tests)
endif ( BUILD_TESTING )




