cmake_minimum_required(VERSION 3.16)

project(3d_viewer VERSION 0.1 LANGUAGES CXX)

set(CFLAGS "-Wall -Werror -Wextra")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
    main.cc
    backend/backend.cc
    backend/controller.cc
    frontend/frontend.cc
    frontend/file_loader.cc
    frontend/range_input.cc
)

set(PROJECT_HEADERS
    frontend/frontend.h
    backend/backend.h
    backend/controller.h
    backend/constants.h
    frontend/file_loader.h
    frontend/range_input.h
)

option(WITH_GIF_SUPPORT "Build with Gif support" OFF)

if(WITH_GIF_SUPPORT)
    find_package(GIF REQUIRED)
    list(APPEND PROJECT_SOURCES
        frontend/gif_recorder.cc)
    list(APPEND PROJECT_HEADERS
        frontend/gif_recorder.h)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3d_viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
    else()
    if(ANDROID)
        add_library(3d_viewer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(3d_viewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(3d_viewer PRIVATE
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui)

if(WITH_GIF_SUPPORT)
    target_link_libraries(3d_viewer PRIVATE
        ${GIF_LIBRARY})
    target_compile_definitions(3d_viewer PRIVATE WITH_GIF_SUPPORT)
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qt)
endif()
set_target_properties(3d_viewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS 3d_viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3d_viewer)
endif()
