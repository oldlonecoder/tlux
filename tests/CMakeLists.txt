include (FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.0
)


FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(tlux.stracc tlux.stracc.cc)

target_include_directories(tlux.stracc PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_link_libraries(tlux.stracc PRIVATE tlux GTest::GTest )

add_dependencies(tlux.stracc tlux)
add_test(NAME tlux COMMAND tlux.stracc)
