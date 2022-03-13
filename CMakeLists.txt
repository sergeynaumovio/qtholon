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
    PRIVATE src/plugin.cpp
            src/core/qtholonglobal.h
            src/core/holoncore.h
            src/core/holoncore.cpp
            src/core/holontask.h
            src/core/holontask.cpp

            src/widgets/holondesktop.h
            src/widgets/holondesktop.cpp
            src/widgets/holontaskbar.h
            src/widgets/holontaskbar.cpp
            src/widgets/holonnewtaskmenu.h
            src/widgets/holonnewtaskmenu.cpp
            src/widgets/holonopentasks.h
            src/widgets/holonopentasks.cpp
            src/widgets/holonsidebar.h
            src/widgets/holonsidebar.cpp
            src/widgets/holonsidebarswitch.h
            src/widgets/holonsidebarswitch.cpp
            src/widgets/holonwindow.h
            src/widgets/holonwindow.cpp
            src/widgets/holonwindowarea.h
            src/widgets/holonwindowarea.cpp
            src/widgets/holonwindowarea_p.h
            src/widgets/holonwindowarea_p.cpp
            src/widgets/holonnewwindowmenu.h
            src/widgets/holonnewwindowmenu.cpp
)

target_include_directories(${QT_HOLON_LIBRARY}
    PRIVATE src/core
            src/widgets
)

set(QT_HOLON_PUBLIC_HEADERS
    src/core/qtholonglobal.h
    src/core/HolonCore
    src/core/holoncore.h
    src/core/HolonTask
    src/core/holontask.h
    src/widgets/HolonDesktop
    src/widgets/holondesktop.h
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
