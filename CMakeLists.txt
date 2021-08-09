cmake_minimum_required(VERSION 3.5)

project(TextEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES TextEditor_es_DO.ts)
set(RESOURCE_FILES icons.qrc)

configure_file(src/textEditorConfig.h.in src/textEditorConfig.h)

set(PROJECT_SOURCES
        src/main.cpp
        src/texteditor.cpp
        src/texteditor.h
        src/texteditor.ui
        src/settings.cpp
        src/settings.h
        src/${TS_FILES}
        icons/${RESOURCE_FILES}
)

add_executable(TextEditor ${PROJECT_SOURCES})
qt6_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_include_directories(TextEditor PUBLIC src"${PROJECT_BINARY_DIR}")
target_link_libraries(TextEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TextEditor)
endif()
