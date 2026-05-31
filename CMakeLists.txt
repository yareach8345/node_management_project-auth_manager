cmake_minimum_required(VERSION 4.2)
project(auth_manager)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(auth_manager main.cpp)
target_link_libraries(auth_manager
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

