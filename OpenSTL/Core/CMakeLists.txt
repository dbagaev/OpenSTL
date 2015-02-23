file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(Core SHARED ${ALL_SOURCE})
add_library(OpenSTL.Core ALIAS Core)

set_target_properties(Core PROPERTIES OUTPUT_NAME openstl-core)
