cmake_minimum_required(VERSION 3.5)

project(TextEditor VERSION 0.1 LANGUAGES CXX DESCRIPTION "A Simple Text Editor")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6Core)

add_subdirectory(src)

configure_file(src/main.cpp.in main.cpp @ONLY)

add_executable(TextEditor WIN32 MACOSX_BUNDLE
    main.cpp
    icons/icons.qrc
)

target_link_libraries(TextEditor PRIVATE libTEXTEDITOR)


set_target_properties(TextEditor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER texteditor.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

