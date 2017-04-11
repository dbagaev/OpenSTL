file(GLOB ALL_SOURCE *.h *.cpp)

source_group("\\" FILES ${ALL_SOURCE})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBRARIES})

add_executable(Data.Tests ${ALL_SOURCE})
add_executable(mesh.Data.Tests ALIAS Data.Tests)

target_link_libraries(Data.Tests mesh.Data)

target_link_libraries(Data.Tests gtest)

if ( UNIX )
    target_link_libraries(Data.Tests pthread)
endif ( UNIX )

set_target_properties(Data.Tests PROPERTIES OUTPUT_NAME mesh-data-tests)

# Run tests after build
get_target_property( TEST_RUNTIME_OUTPUT_DIRECTORY Data.Tests RUNTIME_OUTPUT_DIRECTORY_DEBUG )
add_custom_command ( TARGET Data.Tests
                     POST_BUILD
                     COMMAND $<TARGET_FILE:Data.Tests> )


