cmake_minimum_required(VERSION 3.5)

project(TextEditor VERSION 0.2 LANGUAGES CXX DESCRIPTION "A Simple Text Editor")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

configure_file(src/main.cpp.in main.cpp @ONLY)

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()

include(GNUInstallDirs)
add_executable(TextEditor WIN32 MACOSX_BUNDLE
    main.cpp
    icons/icons.qrc
)

target_link_libraries(TextEditor PRIVATE libTEXTEDITOR)
install(TARGETS TextEditor
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

set_target_properties(TextEditor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER texteditor.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_include_directories(TextEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

message("CMAKE_CURRENT_SOURCE_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/src")

##CPack
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "$(_qmake_executable)" DIRECTORY)

set(CPACK_PACKAGE_NAME "TextEditor")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TextEditor")
set(CPACK_PACKAGE_VERSION "1.0.0")
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    install(FILES ${CMAKE_SOURCE_DIR}/editor.desktop DESTINATION share/applications/)
    install(FILES ${CMAKE_SOURCE_DIR}/editor.png DESTINATION share/pixamps/)
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-linux-${ARCHITECTURE}")
    set(CPACK_GENERATOR "TXZ")

elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "TextEditor")
    set(CPACK_NSIS_DISPLAY_NAME ${CMAKE_PACKAGE_NAME})
    set(CPACK_NSIS_COMPRESSOR lzma)
    set(CPACK_NSIS_INSTALLED_ICON_NAME TextEditor.exe)
    set(CPACK_NSIS_MENU_LINKS "TextEditor.exe" "TextEditor")
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR})
    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)

    include(InstallRequiredSystemLibraries)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET TextEditor POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/qtDeploy/
                        COMMAND ${WINDEPLOYQT_EXECUTABLE}
                               --release
                               --verbose 1
                               --no-compiler-runtime
                               --no-angle
                               --no-opengl
                               --no-opengl-sw
                               --no-webkit2
                               --no-quick-import
                               --no-translations
                               --dir ${CMAKE_BINARY_DIR}/qtDeploy $<TARGET_FILE:TextEditor>
        )
        install(
            DIRECTORY ${CMAKE_BINARY_DIR}/qtDeploy/
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            FILES_MATCHING PATTERN "*.*"
        )
        set(CPACK_GENERATOR "ZIP;NSIS")

elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")

    find_program(MACDEPLOYQT macdeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(
            TARGET TextEditor POST_BUILD
            COMMAND ${MACDEPLOYQT} TextEditor.app
     )
     set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-macos")
     set(CPACK_DMG_VOLUME_NAME "TextEditor")
endif()

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-src")
set(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
include(CPack)
