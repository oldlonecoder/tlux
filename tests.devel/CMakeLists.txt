set(This tlux.app)

add_executable(${This} ${This}.cc)


target_include_directories(${This} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_link_libraries(${This} PRIVATE tlux )

add_dependencies(${This} tlux)




