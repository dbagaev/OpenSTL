file(GLOB ALL_SOURCE *.h *.cpp)
file(GLOB OCL_KERNELS ocl_kernels/*.cl)

source_group("\\" FILES ${ALL_SOURCE})
source_group("\\ocl_kernels" FILES ${OCL_KERNELS})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})

add_library(Algorithm SHARED ${ALL_SOURCE} ${OCL_KERNELS})
add_library(OpenSTL.Algorithm ALIAS Algorithm)

target_link_libraries(Algorithm OpenSTL.Math OpenSTL.Data)

find_package(OpenCL)

if(OpenCL_FOUND)
	include_directories(${OpenCL_INCLUDE_DIR})
	target_link_libraries(Algorithm ${OpenCL_LIBRARY})
endif()

set_target_properties(Algorithm PROPERTIES OUTPUT_NAME openstl-algorithm)
