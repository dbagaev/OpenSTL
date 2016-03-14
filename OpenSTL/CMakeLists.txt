include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Core)
add_subdirectory(Math)
add_subdirectory(Data)

option(BUILD_PYTHON_MODULES "Enable building of Python modules" OFF)

if (BUILD_PYTHON_MODULES)
    add_subdirectory(Python)
endif(BUILD_PYTHON_MODULES)