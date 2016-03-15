file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})

add_library(Algorithm STATIC ${ALL_SOURCE})
add_library(OpenSTL.Algorithm ALIAS Algorithm)

find_package(OpenCL)

if(OpenCL_FOUND)
	include_directories(${OpenCL_INCLUDE_DIR})
	target_link_libraries(Algorithm ${OpenCL_LIBRARY})
endif()

set_target_properties(Algorithm PROPERTIES OUTPUT_NAME openstl-algorithm)
