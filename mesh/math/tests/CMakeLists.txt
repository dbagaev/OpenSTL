file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBRARIES})

add_executable(Math.Tests ${ALL_SOURCE})
add_executable(mesh.Math.Tests ALIAS Math.Tests)

target_link_libraries(Math.Tests mesh.Math)

target_link_libraries(Math.Tests gtest)

if ( UNIX )
    target_link_libraries(Math.Tests pthread)
endif ( UNIX )

set_target_properties(Math.Tests PROPERTIES OUTPUT_NAME mesh-math-tests)

# Run tests after build
get_target_property( TEST_RUNTIME_OUTPUT_DIRECTORY Math.Tests RUNTIME_OUTPUT_DIRECTORY_DEBUG )
add_custom_command ( TARGET Math.Tests
                     POST_BUILD
                     COMMAND $<TARGET_FILE:Math.Tests> )


