
set(This tuxls_database)

add_executable(${This} ${This}.cc)


target_include_directories(${This} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_link_libraries(${This} PRIVATE tuxls GTest::GTest )

add_dependencies(${This} tuxls)

include(GoogleTest)

GTEST_ADD_TESTS(${This} "" AUTO )

add_test(NAME ${This} COMMAND ${This})
