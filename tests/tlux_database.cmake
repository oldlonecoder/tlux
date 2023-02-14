
set(This tlux_database)

add_executable(${This} ${This}.cc)


target_include_directories(${This} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_link_libraries(${This} PRIVATE tlux GTest::GTest )

add_dependencies(${This} tlux)

include(GoogleTest)

GTEST_ADD_TESTS(${This} "" AUTO )

add_test(NAME ${This} COMMAND ${This})
