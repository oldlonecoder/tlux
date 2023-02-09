#project(tuxic VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )


add_library(
        ${PROJECT_NAME}
        SHARED
        # include/${FrameWorkName}/dlconfig.h     #global framework-wide macros definitions and dll export and import macros for MSVC.
        include/${PROJECT_NAME}/textattr.h         src/textattr.cc
        #include/utf/Icons.h        src/utf/Icons.cc
        #include/utf/accents.fr.h   src/utf/accents.fr.cc
        #include/stracc.h           src/stracc.cc #include the above headers.
        #include/application.h      src/application.cc
        #include/diagnostic.h       src/diagnostic.cc #include the above headers.
        #include/geometry.h         src/geometry.cc # standalone
        #include/teacc_signal.h  # bare basic single-thread signal-slots
        #include/strbrk.h           src/strbrk.cc
        #include/object.h           src/object.cc
#        include/${FrameWorkName}/db/entity.h        src/db/entity.cc
#        include/${FrameWorkName}/db/database.h      src/db/database.cc

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

