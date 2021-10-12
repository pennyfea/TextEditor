find_package(Qt5 5.15 COMPONENTS Widgets Core Gui REQUIRED)

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

target_link_libraries(libTEXTEDITOR PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui)
