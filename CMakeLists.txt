cmake_minimum_required(VERSION 3.15)

project("Qt Holon" LANGUAGES CXX)

if(MSVC)
    cmake_policy(SET CMP0092 NEW) # Disable passing /W3 by default on MSVC
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}Loader)

if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

# QT_HOLON_LIBRARY -------------------------------------------------------------
include("cmake/QtHolonVersion.in")
set(QT_HOLON_LIBRARY Qt${QT_VERSION_MAJOR}Holon)

add_library(${QT_HOLON_LIBRARY} SHARED)

target_sources(${QT_HOLON_LIBRARY}
    PRIVATE src/core/holonabstractitemmodel.cpp
            src/core/holonabstractitemmodel.h
            src/core/holoncore.cpp
            src/core/holoncore.h
            src/core/holoncore_p.h
            src/core/holonnamespace.h
            src/core/holontaskmodel.cpp
            src/core/holontaskmodel.h
            src/core/holontaskmodelbranch.cpp
            src/core/holontaskmodelbranch.h
            src/core/holonworkflowmodel.cpp
            src/core/holonworkflowmodel.h
            src/core/holonworkflowmodelbranch.cpp
            src/core/holonworkflowmodelbranch.h
            src/core/qtholonglobal.h
            src/tasks/holonabstracttask.cpp
            src/tasks/holonabstracttask.h
            src/tasks/holoncustomtask.cpp
            src/tasks/holoncustomtask.h
            src/widgets/holonabstractwidget.cpp
            src/widgets/holonabstractwidget.h
            src/widgets/holonabstractwindow.cpp
            src/widgets/holonabstractwindow.h
            src/widgets/holondesktop.cpp
            src/widgets/holondesktop.h
            src/widgets/holondesktop_p.cpp
            src/widgets/holondesktop_p.h
            src/widgets/holonmainwindow.cpp
            src/widgets/holonmainwindow.h
            src/widgets/holonparameterswindow.cpp
            src/widgets/holonparameterswindow.h
            src/widgets/holonsettingswindow.cpp
            src/widgets/holonsettingswindow.h
            src/widgets/holonsidebar.cpp
            src/widgets/holonsidebar.h
            src/widgets/holonsidebar_p.cpp
            src/widgets/holonsidebar_p.h
            src/widgets/holonsidebardock.cpp
            src/widgets/holonsidebardock.h
            src/widgets/holonsidebardock_p.cpp
            src/widgets/holonsidebardock_p.h
            src/widgets/holonstackedwidget.cpp
            src/widgets/holonstackedwidget.h
            src/widgets/holontaskbar.cpp
            src/widgets/holontaskbar.h
            src/widgets/holontasklistwindow.cpp
            src/widgets/holontasklistwindow.h
            src/widgets/holontaskmenu.cpp
            src/widgets/holontaskmenu.h
            src/widgets/holontasktreewindow.cpp
            src/widgets/holontasktreewindow.h
            src/widgets/holonterminalwindow.cpp
            src/widgets/holonterminalwindow.h
            src/widgets/holonwindowarea.cpp
            src/widgets/holonwindowarea.h
            src/widgets/holonwindowarea_p.cpp
            src/widgets/holonwindowarea_p.h
            src/widgets/holonwindowareaswitch.cpp
            src/widgets/holonwindowareaswitch.h
            src/widgets/holonwindowmenu.cpp
            src/widgets/holonwindowmenu.h
            src/widgets/holonworkflowwindow.cpp
            src/widgets/holonworkflowwindow.h
            src/plugin.cpp
)

target_include_directories(${QT_HOLON_LIBRARY}
    PRIVATE src/core
            src/tasks
            src/widgets
)

set(QT_HOLON_PUBLIC_HEADERS
    src/core/HolonAbstractItemModel
    src/core/HolonCore
    src/core/HolonTaskModel
    src/core/HolonWorkflowModel
    src/core/holonabstractitemmodel.h
    src/core/holoncore.h
    src/core/holonnamespace.h
    src/core/holontaskmodel.h
    src/core/holonworkflowmodel.h
    src/core/qtholonglobal.h
    src/tasks/HolonAbstractTask
    src/tasks/holonabstracttask.h
    src/widgets/HolonAbstractWindow
    src/widgets/HolonDesktop
    src/widgets/HolonSidebar
    src/widgets/holonabstractwindow.h
    src/widgets/holondesktop.h
    src/widgets/holonsidebar.h
)

set_target_properties(${QT_HOLON_LIBRARY} PROPERTIES
    OUTPUT_NAME ${QT_HOLON_LIBRARY}
    SOVERSION ${QT_VERSION_MAJOR}
    VERSION ${QT_HOLON_VERSION}
    PUBLIC_HEADER "${QT_HOLON_PUBLIC_HEADERS}"
)

target_link_libraries(${QT_HOLON_LIBRARY}
    PRIVATE Qt${QT_VERSION_MAJOR}Loader
)

target_compile_definitions(${QT_HOLON_LIBRARY}
    PRIVATE QT_BUILD_HOLON_LIB
)

qt_add_resources(${QT_HOLON_LIBRARY} "Holon"
    PREFIX "/holon"
    BASE "share/icons"
    FILES share/icons/holoniconlight.svg
          share/icons/quit.svg
          share/icons/screwdriver.svg
)

include(GNUInstallDirs)
set(QT_HOLON_INCLUDEDIR qt${QT_VERSION_MAJOR}/QtHolon)
install(TARGETS ${QT_HOLON_LIBRARY}
    EXPORT ${QT_HOLON_LIBRARY}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT QtHolon_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT QtHolon_Runtime
            NAMELINK_COMPONENT QtHolon_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT QtHolon_Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QT_HOLON_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QT_HOLON_INCLUDEDIR}
                  COMPONENT QtHolon_Development
)

install(EXPORT ${QT_HOLON_LIBRARY}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${QT_HOLON_LIBRARY}
    FILE Qt${QT_VERSION_MAJOR}HolonTargets.cmake
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/QtHolonConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${QT_HOLON_LIBRARY}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${QT_HOLON_LIBRARY}
    PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${QT_HOLON_LIBRARY}Version.cmake
    VERSION ${QT_HOLON_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${QT_HOLON_LIBRARY}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/${QT_HOLON_LIBRARY}Version.cmake"
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${QT_HOLON_LIBRARY}"
)
