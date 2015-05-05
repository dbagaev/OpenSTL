cmake_minimum_required(VERSION 3.1)
project(OpenSTL)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )


if ( MSVC )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/lib )

    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/lib )

endif ( MSVC )

if ( UNIX )
    set( CMAKE_CXX_FLAGS -std=c++11 ${CMAKE_CXX_FLAGS} )
endif ( UNIX )

set ( BUILD_UNIT_TESTS CACHE TRUE BOOL "Build unit tests on GTest platform" )

find_package( Boost )

if ( BUILD_UNIT_TESTS )

    find_package( GTest )

    if ( NOT GTEST_FOUND )
        set ( GTEST_INCLUDE_DIRS $ENV{GTEST_INCLUDE_DIRS} )
        if ( MSVC )
            set ( GTEST_LIBRARIES "$ENV{GTEST_LIBRARY}/$(Platform)/$(Configuration)" )
        else ( MSVC )
            set ( GTEST_LIBRARIES "$ENV{GTEST_LIBRARY}" )
        endif ( MSVC )
    endif( NOT GTEST_FOUND )
endif ( BUILD_UNIT_TESTS )

add_subdirectory(OpenSTL )
add_subdirectory(Tests )
add_subdirectory(Demo )