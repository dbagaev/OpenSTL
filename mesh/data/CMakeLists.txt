file(GLOB ALL_SOURCE *.h *.cpp)
file(GLOB DETAILS_SOURCE details/*.h details/*.cpp)

source_group("\\" FILES ${ALL_SOURCE})
source_group("\\Details" FILES ${DETAILS_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_definitions(-D__BUILDING_mesh_DATA)

add_library(Data SHARED ${ALL_SOURCE} ${DETAILS_SOURCE})
add_library(mesh.Data ALIAS Data)

target_link_libraries(Data mesh.Math)

set_target_properties(Data PROPERTIES OUTPUT_NAME mesh-data)

if ( BUILD_TESTING )
	add_subdirectory(Tests)
endif ( BUILD_TESTING )
	