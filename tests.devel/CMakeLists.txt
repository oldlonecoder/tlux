set(This tuxls.app)

add_executable(${This} ${This}.cc)


target_include_directories(${This} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_link_libraries(${This} PRIVATE tuxls )

add_dependencies(${This} tuxls)




