file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

find_package(PythonLibs REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
link_directories(${LINK_DIRECTORIES} ${Boost_LIB_DIR} ${Python_LIB_DIR})

add_library(Python SHARED ${ALL_SOURCE})
add_library(OpenSTL.Python ALIAS Python)
target_link_libraries(Python OpenSTL.Data)

set_target_properties(Python PROPERTIES OUTPUT_NAME py-openstl)
