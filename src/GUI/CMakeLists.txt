cmake_minimum_required(VERSION 3.20.4 FATAL_ERROR)

set(GUI_SOURCES
        GUIButton.cpp
        GUIEnums.cpp
        GUIFont.cpp
        GUIProgressBar.cpp
        GUIWindow.cpp)

set(GUI_HEADERS
        GUIButton.h
        GUIDialogues.h
        GUIEnums.h
        GUIFont.h
        GUIProgressBar.h
        GUIWindow.h)

add_library(gui STATIC ${GUI_SOURCES} ${GUI_HEADERS})
target_check_style(gui)

target_link_libraries(gui
                      gui_ui
                      utility)

add_subdirectory(UI)
