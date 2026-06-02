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

find_package(OpenSSL REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(auth_manager src/main.cpp
        include/auth/root_key_manager/IRootKeyManager.h
        include/auth/root_key_manager/RootKeyManagerOpenSSLImpl.h
        src/auth/root_key_manager/RootKeyManagerOpenSSLImpl.cpp
        include/auth/config/AuthConfig.h
        src/auth/config/AuthConfig.cpp
)

target_include_directories(auth_manager
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(auth_manager
        Qt::Core
        Qt::Gui
        Qt::Widgets
        OpenSSL::SSL
        OpenSSL::Crypto
        yaml-cpp::yaml-cpp
)

# config 파일 위치
target_compile_definitions(auth_manager PRIVATE CONFIG_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/config.yaml")
