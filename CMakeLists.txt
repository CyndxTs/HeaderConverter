cmake_minimum_required(VERSION 4.0)
project(HeaderConverter)
enable_language(RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(HeaderConverter main.cpp
        converter.cpp
        converter.h
        declarations.cpp
        declarations.h
        gui.cpp
        gui.h
        app.rc)
target_link_libraries(HeaderConverter
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

if(WIN32)
    set_target_properties(HeaderConverter PROPERTIES WIN32_EXECUTABLE ON)
endif()