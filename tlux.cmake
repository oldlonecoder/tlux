#project(tlux VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )


add_library(
        ${PROJECT_NAME}
        SHARED
        # include/${FrameWorkName}/dlconfig.h     #global framework-wide macros definitions and dll export and import macros for MSVC.
        include/${PROJECT_NAME}/textattr.h         src/textattr.cc
        include/${PROJECT_NAME}/utf/Icons.h        src/utf/Icons.cc
        include/${PROJECT_NAME}/utf/accents.fr.h   src/utf/accents.fr.cc
        include/${PROJECT_NAME}/stracc.h           src/stracc.cc #include the above headers.
        include/${PROJECT_NAME}/application.h      src/application.cc
        include/${PROJECT_NAME}/diagnostic.h       src/diagnostic.cc #include the above headers.
        include/${PROJECT_NAME}/geometry.h         src/geometry.cc # standalone
        include/${PROJECT_NAME}/signals.h  # bare basic single-thread signal-slots
        include/${PROJECT_NAME}/strbrk.h           src/strbrk.cc
        include/${PROJECT_NAME}/object.h           src/object.cc
        include/${PROJECT_NAME}/stml/text.h        src/stml/text.cc
        include/${PROJECT_NAME}/db/field.h         src/db/field.cc
        include/${PROJECT_NAME}/db/table.h         src/db/table.cc
        include/${PROJECT_NAME}/db/database.h      src/db/database.cc
        include/${PROJECT_NAME}/argc.h             src/argc.cc
)


target_compile_definitions(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${${PROJECT_NAME}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            )
ENDIF()

target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        )

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} sqlite3)

add_executable(tests.app
	tests/test.h
	tests/test.cc
)

target_link_libraries(tests.app sqlite3 tlux ${CMAKE_DL_LIBS})


install(DIRECTORY
        include/${PROJECT_NAME}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}"
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)


