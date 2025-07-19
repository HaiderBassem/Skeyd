cmake_minimum_required(VERSION 3.16)

project(Skeyd VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# إضافة PkgConfig
find_package(PkgConfig REQUIRED)

# البحث عن مكتبة xkbcommon
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)

# البحث عن مكتبة libudev
pkg_check_modules(LIBUDEV REQUIRED libudev)

# البحث عن مكتبة libinput
pkg_check_modules(LIBINPUT REQUIRED libinput)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Skeyd
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        skeydmanager.h skeydmanager.cpp
        keyboardwidget.h keyboardwidget.cpp
        events.h events.cpp
        eventsworker.h









    )
else()
    if(ANDROID)
        add_library(Skeyd SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Skeyd
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# إضافة include directories للمكتبات الخارجية
target_include_directories(Skeyd PRIVATE
    ${XKBCOMMON_INCLUDE_DIRS}
    ${LIBUDEV_INCLUDE_DIRS}
    ${LIBINPUT_INCLUDE_DIRS}
)

# إضافة link libraries
target_link_libraries(Skeyd PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${XKBCOMMON_LIBRARIES}
    ${LIBUDEV_LIBRARIES}
    ${LIBINPUT_LIBRARIES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Skeyd)
endif()
set_target_properties(Skeyd PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Skeyd
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Skeyd)
endif()
