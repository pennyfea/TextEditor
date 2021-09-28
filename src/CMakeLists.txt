find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_library(libTEXTEDITOR STATIC "")
target_sources(libTEXTEDITOR PRIVATE
    texteditor.cpp
    texteditor.h
    texteditor.ui
    settings.h
    settings.cpp
    settingsdialog.h
    settingsdialog.cpp
    settingsdialog.ui
)

target_link_libraries(libTEXTEDITOR PUBLIC Qt6::Widgets)
