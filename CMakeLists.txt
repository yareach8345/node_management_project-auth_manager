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
find_package(nlohmann_json REQUIRED)

# 라이브러리 선언
add_library(auth_manager_lib
        include/auth_manager/auth/config/AuthConfig.h
        src/auth/config/AuthConfig.cpp
        include/auth_manager/auth/service/IKeyService.h
        include/auth_manager/auth/service/KeyServiceOpenSSLImpl.h
        src/auth/service/KeyServiceOpenSSLImpl.cpp
        include/auth_manager/core/json/IJsonConverter.h
        include/auth_manager/auth/mapping/KeysInfoJsonConverter.h
        src/auth/mapping/KeysInfoJsonConverter.cpp
        include/auth_manager/core/json/JsonFileManager.h
        include/auth_manager/auth/key_provider/KeyProvider.h
        include/auth_manager/auth/key_manager/AuthKeyManager.h
        include/auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h
        src/auth/key_provider/KeyProviderOpenSSLImpl.cpp
        include/auth_manager/auth/util/OpenSSLUtil.h
        src/auth/util/OpenSSLUtil.cpp
        include/auth_manager/gui/GuiWidget.h
        src/gui/GuiWidget.cpp
        include/auth_manager/gui/TabInfo.h
        include/auth_manager/auth/tab/RootKeyManageTab.h
        src/auth/tab/RootKeyManageTab.cpp
        src/auth/key_manager/AuthKeyManager.cpp
)

target_include_directories(auth_manager_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(auth_manager_lib
        PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
        yaml-cpp::yaml-cpp
        nlohmann_json::nlohmann_json
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# 실행 파일 선언
add_executable(auth_manager
        src/main.cpp
)

target_link_libraries(auth_manager
        PRIVATE
        auth_manager_lib
)

# config 파일 위치
target_compile_definitions(auth_manager PRIVATE CONFIG_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/config.yaml")

# 테스트 설정
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(auth_manager_test
        test/test.cpp
        test/unit/KeyProviderTest.cpp
)

target_link_libraries(auth_manager_test
        PRIVATE
        auth_manager_lib
        GTest::gtest GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(auth_manager_test)