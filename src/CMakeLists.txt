find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_library(libTEXTEDITOR STATIC "")
target_sources(libTEXTEDITOR PRIVATE
    texteditor.cpp
    texteditor.h
    texteditor.ui
    settings.cpp
    settings.h
)

target_link_libraries(libTEXTEDITOR PUBLIC Qt6::Widgets)
