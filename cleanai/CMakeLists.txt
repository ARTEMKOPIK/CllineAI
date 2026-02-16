cmake_minimum_required(VERSION 3.26)
project(CleanAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
    message(FATAL_ERROR "CleanAI поддерживает только Windows 11 22H2+")
endif()

option(CLEANAI_HEADLESS_CI "Build headless CI stub instead of WinUI app" OFF)

add_compile_definitions(
    UNICODE
    _UNICODE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

find_package(cpprestsdk REQUIRED)
find_package(SQLite3 REQUIRED)

if(CLEANAI_HEADLESS_CI)
    add_executable(CleanAI src/ci_main.cpp)

    target_link_libraries(CleanAI PRIVATE
        cpprestsdk::cpprest
        SQLite::SQLite3
    )

    add_executable(CleanAI.CoreLogicTests
        tests/CoreLogicTests.cpp
        src/Core/DiskScanner.cpp
        src/Core/OllamaClient.cpp
    )

    target_include_directories(CleanAI.CoreLogicTests PRIVATE src)

    target_link_libraries(CleanAI.CoreLogicTests PRIVATE
        cpprestsdk::cpprest
        SQLite::SQLite3
    )

    enable_testing()
    add_test(NAME CoreLogicTests COMMAND CleanAI.CoreLogicTests)

    install(TARGETS CleanAI RUNTIME DESTINATION .)
    return()
endif()

set(WINDOWS_APP_SDK_VERSION "1.5")
find_package(Microsoft.WindowsAppSDK REQUIRED CONFIG)

set(CLEANAI_SOURCES
    src/main.cpp
    src/App.cpp
    src/MainWindow.cpp
    src/Pages/OnboardingPage.cpp
    src/Pages/ScanPage.cpp
    src/Pages/TreeMapPage.cpp
    src/Pages/RecommendationsPage.cpp
    src/Pages/HistoryPage.cpp
    src/Core/DiskScanner.cpp
    src/Core/TreemapLayout.cpp
    src/Core/OllamaClient.cpp
    src/Core/QuarantineManager.cpp
    src/Core/HistoryDatabase.cpp
)

set(CLEANAI_HEADERS
    src/App.h
    src/MainWindow.h
    src/Pages/OnboardingPage.h
    src/Pages/ScanPage.h
    src/Pages/TreeMapPage.h
    src/Pages/RecommendationsPage.h
    src/Pages/HistoryPage.h
    src/Core/DiskScanner.h
    src/Core/TreemapLayout.h
    src/Core/OllamaClient.h
    src/Core/QuarantineManager.h
    src/Core/HistoryDatabase.h
    src/Models/FileItem.h
    src/Models/Recommendation.h
)

set(CLEANAI_XAML
    src/App.xaml
    src/MainWindow.xaml
    src/Pages/OnboardingPage.xaml
    src/Pages/ScanPage.xaml
    src/Pages/TreeMapPage.xaml
    src/Pages/RecommendationsPage.xaml
    src/Pages/HistoryPage.xaml
)

add_executable(CleanAI WIN32 ${CLEANAI_SOURCES} ${CLEANAI_HEADERS} ${CLEANAI_XAML})

target_include_directories(CleanAI PRIVATE src)

target_link_libraries(CleanAI PRIVATE
    Microsoft.WindowsAppSDK::Microsoft.WindowsAppSDK
    cpprestsdk::cpprest
    SQLite::SQLite3
    windowsapp
)

set_target_properties(CleanAI PROPERTIES
    VS_GLOBAL_ApplicationManifest "${CMAKE_SOURCE_DIR}/app.manifest"
    VS_GLOBAL_RootNamespace "CleanAI"
)

configure_file(Package.appxmanifest Package.appxmanifest COPYONLY)

install(TARGETS CleanAI RUNTIME DESTINATION .)
