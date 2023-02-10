project(scripter VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(TargetName "${FrameWorkName}_${PROJECT_NAME}")

add_library(
        ${TargetName}
        SHARED
        include/${FrameWorkName}/${PROJECT_NAME}/types.h src/${PROJECT_NAME}/types.cc
        include/${FrameWorkName}/${PROJECT_NAME}/lexer.h src/${PROJECT_NAME}/lexer.cc
        include/${FrameWorkName}/${PROJECT_NAME}/tokendata.h src/${PROJECT_NAME}/tokendata.cc
        include/${FrameWorkName}/${PROJECT_NAME}/lexer_color.h src/${PROJECT_NAME}/lexer_color.cc
        include/${FrameWorkName}/${PROJECT_NAME}/alu.h src/${PROJECT_NAME}/alu.cc
        include/${FrameWorkName}/${PROJECT_NAME}/xio.h src/${PROJECT_NAME}/xio.cc
        include/${FrameWorkName}/${PROJECT_NAME}/grammar.h src/${PROJECT_NAME}/grammar.cc
        include/${FrameWorkName}/${PROJECT_NAME}/variable.h src/${PROJECT_NAME}/variable.cc
        include/${FrameWorkName}/${PROJECT_NAME}/bloc.h src/${PROJECT_NAME}/bloc.cc
)


target_compile_definitions(${TargetName} PUBLIC "${TargetName}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${TargetName} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${TargetName} EXPORT_FILE_NAME ${${TargetName}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            )
ENDIF()

target_include_directories(${TargetName} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        $<INSTALL_INTERFACE:include/${FrameWorkName}/${PROJECT_NAME}>
        )

target_link_libraries(${TargetName} ${CMAKE_DL_LIBS} sqlite3 teacc_main teacc_stml)


install(DIRECTORY
        include/${FrameWorkName}/${PROJECT_NAME}/
        DESTINATION DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${FrameWorkName}/${PROJECT_NAME}"
)

install(TARGETS ${TargetName}
        EXPORT ${TargetName}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

