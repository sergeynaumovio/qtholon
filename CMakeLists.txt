# Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
# SPDX-License-Identifier: 0BSD

cmake_minimum_required(VERSION 3.18)

project(QtHolon LANGUAGES CXX)

if(MSVC)
    cmake_policy(SET CMP0092 NEW) # Disable passing /W3 by default on MSVC
    add_compile_options(/W4 /WX)
else()
    if(MSVC)
        add_compile_options(-Wall -Werror -Wextra)
    else()
        add_compile_options(-Wall -Werror)
    endif()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

find_package(QT NAMES Qt6)
find_package(Qt${QT_VERSION_MAJOR} 6.8 REQUIRED COMPONENTS QuickWidgets)
find_package(Qt${QT_VERSION_MAJOR}Loader)

if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

# QT_HOLON_LIBRARY -------------------------------------------------------------
include("cmake/QtHolonConfigVersion.in")
set(QT_HOLON_LIBRARY Qt${QT_VERSION_MAJOR}Holon)

add_library(${QT_HOLON_LIBRARY} SHARED)

target_compile_definitions(${QT_HOLON_LIBRARY}
    PRIVATE QT_NO_CAST_FROM_ASCII
            QT_LEAN_HEADERS
)

target_sources(${QT_HOLON_LIBRARY}
    PRIVATE src/core/holoncore.cpp
            src/core/holoncore.h
            src/core/holoncore_p.h
            src/core/holonid.cpp
            src/core/holonid.h
            src/core/holonnamespace.h
            src/core/holonopentasktreemodel.cpp
            src/core/holonopentasktreemodel.h
            src/core/holontaskfolder.cpp
            src/core/holontaskfolder.h
            src/core/holontaskfoldermodel.cpp
            src/core/holontaskfoldermodel.h
            src/core/holonworkflow.cpp
            src/core/holonworkflow.h
            src/core/holonworkflow_p.cpp
            src/core/holonworkflow_p.h
            src/core/holonworkflowitem.cpp
            src/core/holonworkflowitem.h
            src/core/holonworkflowmodel.cpp
            src/core/holonworkflowmodel.h
            src/core/qtholonglobal.h
            src/pyside/holonpythonbindings.cpp
            src/pyside/holonpythonbindings.h
            src/pyside/holonpythontask.cpp
            src/pyside/holonpythontask.h
            src/pyside/holonpythontaskwindow.cpp
            src/pyside/holonpythontaskwindow.h
            src/tasks/holonabstractcondition.cpp
            src/tasks/holonabstractcondition.h
            src/tasks/holonabstracttask.cpp
            src/tasks/holonabstracttask.h
            src/tasks/holonabstracttask_p.cpp
            src/tasks/holonabstracttask_p.h
            src/tasks/holoncustomtask.cpp
            src/tasks/holoncustomtask.h
            src/tasks/holontaskthread.cpp
            src/tasks/holontaskthread.h
            src/widgets/holonabstracttaskwindow.cpp
            src/widgets/holonabstracttaskwindow.h
            src/widgets/holonabstracttaskwindow_p.cpp
            src/widgets/holonabstracttaskwindow_p.h
            src/widgets/holonabstractwindow.cpp
            src/widgets/holonabstractwindow.h
            src/widgets/holonabstractwindow_p.cpp
            src/widgets/holonabstractwindow_p.h
            src/widgets/holondesktop.cpp
            src/widgets/holondesktop.h
            src/widgets/holondesktop_p.cpp
            src/widgets/holondesktop_p.h
            src/widgets/holondockwidget.cpp
            src/widgets/holondockwidget.h
            src/widgets/holondockwidgetsplitstate.cpp
            src/widgets/holondockwidgetsplitstate.h
            src/widgets/holonmainwindow.cpp
            src/widgets/holonmainwindow.h
            src/widgets/holonmarkupeditorwindow.cpp
            src/widgets/holonmarkupeditorwindow.h
            src/widgets/holonmessageswindow.cpp
            src/widgets/holonmessageswindow.h
            src/widgets/holonopentasktreeview.cpp
            src/widgets/holonopentasktreeview.h
            src/widgets/holonopentasktreewindow.cpp
            src/widgets/holonopentasktreewindow.h
            src/widgets/holonparameterswindow.cpp
            src/widgets/holonparameterswindow.h
            src/widgets/holonparameterswindow_p.cpp
            src/widgets/holonparameterswindow_p.h
            src/widgets/holonprojecttasktreewindow.cpp
            src/widgets/holonprojecttasktreewindow.h
            src/widgets/holonprojecttasktreewindow_p.cpp
            src/widgets/holonprojecttasktreewindow_p.h
            src/widgets/holonsettingswindow.cpp
            src/widgets/holonsettingswindow.h
            src/widgets/holonsidebar.cpp
            src/widgets/holonsidebar.h
            src/widgets/holonsidebar_p.cpp
            src/widgets/holonsidebar_p.h
            src/widgets/holonsidebardockwidget.cpp
            src/widgets/holonsidebardockwidget.h
            src/widgets/holonsidebardockwidget_p.cpp
            src/widgets/holonsidebardockwidget_p.h
            src/widgets/holonsidebarmainwindow.cpp
            src/widgets/holonsidebarmainwindow.h
            src/widgets/holonstackedwidget.cpp
            src/widgets/holonstackedwidget.h
            src/widgets/holonstackedwindow.cpp
            src/widgets/holonstackedwindow.h
            src/widgets/holonstackedwindow_p.cpp
            src/widgets/holonstackedwindow_p.h
            src/widgets/holontaskattributeswindow.cpp
            src/widgets/holontaskattributeswindow.h
            src/widgets/holontaskattributeswindow_p.cpp
            src/widgets/holontaskattributeswindow_p.h
            src/widgets/holontaskbar.cpp
            src/widgets/holontaskbar.h
            src/widgets/holontaskstackedwindow.cpp
            src/widgets/holontaskstackedwindow.h
            src/widgets/holontaskstackedwindow_p.cpp
            src/widgets/holontaskstackedwindow_p.h
            src/widgets/holontaskwindowattributeswindow.cpp
            src/widgets/holontaskwindowattributeswindow.h
            src/widgets/holontaskwindowattributeswindow_p.cpp
            src/widgets/holontaskwindowattributeswindow_p.h
            src/widgets/holonterminalwindow.cpp
            src/widgets/holonterminalwindow.h
            src/widgets/holontheme.cpp
            src/widgets/holontheme.h
            src/widgets/holontheme_p.cpp
            src/widgets/holontheme_p.h
            src/widgets/holonthemecolors.cpp
            src/widgets/holonthemecolors.h
            src/widgets/holonthemecolors_p.cpp
            src/widgets/holonthemecolors_p.h
            src/widgets/holonthemeicons.cpp
            src/widgets/holonthemeicons.h
            src/widgets/holonthemeicons_p.cpp
            src/widgets/holonthemeicons_p.h
            src/widgets/holonthemestyle.cpp
            src/widgets/holonthemestyle.h
            src/widgets/holonthemestyle_p.cpp
            src/widgets/holonthemestyle_p.h
            src/widgets/holontitlebar.cpp
            src/widgets/holontitlebar.h
            src/widgets/holontoolbar.cpp
            src/widgets/holontoolbar.h
            src/widgets/holonwindowarea.cpp
            src/widgets/holonwindowarea.h
            src/widgets/holonwindowarea_p.cpp
            src/widgets/holonwindowarea_p.h
            src/widgets/holonwindowareastate.cpp
            src/widgets/holonwindowareastate.h
            src/widgets/holonwindowareaswitch.cpp
            src/widgets/holonwindowareaswitch.h
            src/widgets/holonworkflowgraphicsscene.cpp
            src/widgets/holonworkflowgraphicsscene.h
            src/widgets/holonworkflowwindow.cpp
            src/widgets/holonworkflowwindow.h
            src/plugin.cpp
)

if(NOT MSVC)
    get_target_property(QT_HOLON_SOURCES ${QT_HOLON_LIBRARY} SOURCES)
    set_source_files_properties(${QT_HOLON_SOURCES} PROPERTIES COMPILE_FLAGS -Wextra)
endif()

target_include_directories(${QT_HOLON_LIBRARY}
    PRIVATE src/core
            src/pyside
            src/tasks
            src/widgets
)

set(QT_HOLON_PUBLIC_HEADERS
    src/core/HolonCore
    src/core/HolonTaskFolder
    src/core/HolonWorkflow
    src/core/holoncore.h
    src/core/holonnamespace.h
    src/core/holontaskfolder.h
    src/core/holonworkflow.h
    src/core/qtholonglobal.h
    src/tasks/HolonAbstractCondition
    src/tasks/HolonAbstractTask
    src/tasks/HolonTaskThread
    src/tasks/holonabstractcondition.h
    src/tasks/holonabstracttask.h
    src/tasks/holontaskthread.h
    src/widgets/HolonAbstractTaskWindow
    src/widgets/HolonAbstractWindow
    src/widgets/HolonDesktop
    src/widgets/HolonSidebar
    src/widgets/HolonStackedWindow
    src/widgets/HolonTaskAttributesWindow
    src/widgets/HolonTaskStackedWindow
    src/widgets/HolonTaskWindowAttributesWindow
    src/widgets/HolonTheme
    src/widgets/HolonThemeColors
    src/widgets/HolonThemeIcons
    src/widgets/HolonThemeStyle
    src/widgets/HolonToolBar
    src/widgets/HolonWindowArea
    src/widgets/holonabstracttaskwindow.h
    src/widgets/holonabstractwindow.h
    src/widgets/holondesktop.h
    src/widgets/holonsidebar.h
    src/widgets/holontaskstackedwindow.h
    src/widgets/holonstackedwindow.h
    src/widgets/holontaskattributeswindow.h
    src/widgets/holontaskwindowattributeswindow.h
    src/widgets/holontheme.h
    src/widgets/holonthemecolors.h
    src/widgets/holonthemeicons.h
    src/widgets/holonthemestyle.h
    src/widgets/holontoolbar.h
    src/widgets/holonwindowarea.h
)

set_target_properties(${QT_HOLON_LIBRARY} PROPERTIES
    OUTPUT_NAME ${QT_HOLON_LIBRARY}
    SOVERSION ${QT_VERSION_MAJOR}
    VERSION ${QT_HOLON_VERSION}
    PUBLIC_HEADER "${QT_HOLON_PUBLIC_HEADERS}"
)

target_link_libraries(${QT_HOLON_LIBRARY}
    PRIVATE Qt${QT_VERSION_MAJOR}::QuickWidgets
    PRIVATE Qt${QT_VERSION_MAJOR}Loader
)

target_compile_definitions(${QT_HOLON_LIBRARY}
    PRIVATE QT_BUILD_HOLON_LIB
)

qt_add_resources(${QT_HOLON_LIBRARY} "Holon"
    PREFIX "/holon"
    BASE "share/icons"
    FILES share/icons/clear.xpm
          share/icons/clear@2x.xpm
          share/icons/close.xpm
          share/icons/close@2x.xpm
          share/icons/expand.xpm
          share/icons/expand@2x.xpm
          share/icons/holoniconlight.svg
          share/icons/indicator_arrowdown.xpm
          share/icons/indicator_arrowdown@2x.xpm
          share/icons/indicator_arrowup.xpm
          share/icons/indicator_arrowup@2x.xpm
          share/icons/maximize.xpm
          share/icons/maximize@2x.xpm
          share/icons/minimize.xpm
          share/icons/minimize@2x.xpm
          share/icons/quit.svg
          share/icons/run.xpm
          share/icons/run@2x.xpm
          share/icons/run_task.xpm
          share/icons/run_task@2x.xpm
          share/icons/screwdriver.svg
          share/icons/splitbutton_closebottom.xpm
          share/icons/splitbutton_closebottom@2x.xpm
          share/icons/splitbutton_closeleft.xpm
          share/icons/splitbutton_closeleft@2x.xpm
          share/icons/splitbutton_closeright.xpm
          share/icons/splitbutton_closeright@2x.xpm
          share/icons/splitbutton_closetop.xpm
          share/icons/splitbutton_closetop@2x.xpm
          share/icons/splitbutton_horizontal.xpm
          share/icons/splitbutton_horizontal@2x.xpm
          share/icons/splitbutton_vertical.xpm
          share/icons/splitbutton_vertical@2x.xpm
          share/icons/stop.xpm
          share/icons/stop@2x.xpm
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
    ${CMAKE_CURRENT_BINARY_DIR}/${QT_HOLON_LIBRARY}ConfigVersion.cmake
    VERSION ${QT_HOLON_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${QT_HOLON_LIBRARY}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/${QT_HOLON_LIBRARY}ConfigVersion.cmake"
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${QT_HOLON_LIBRARY}"
)

# PySide setup -----------------------------------------------------------------
# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Use provided python interpreter if given.
if(NOT python_interpreter)
    if(WIN32 AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        find_program(python_interpreter "python_d")
        if(NOT python_interpreter)
            message(FATAL_ERROR
                "A debug Python interpreter could not be found, which is a requirement when "
                "building this example in a debug configuration. Make sure python_d.exe is in "
                "PATH.")
        endif()
    else()
        find_program(python_interpreter "python")
        if(NOT python_interpreter)
            message(FATAL_ERROR
                "No Python interpreter could be found. Make sure python is in PATH.")
        endif()
    endif()
endif()
message(STATUS "Using python interpreter: ${python_interpreter}")

# Macro to get various pyside / python include / link flags.
macro(pyside_config option output_var)
    if(${ARGC} GREATER 2)
        set(is_list ${ARGV2})
    else()
        set(is_list "")
    endif()

    execute_process(
      COMMAND ${python_interpreter} "${CMAKE_SOURCE_DIR}/src/pyside/pyside_config.py"
              ${option}
      OUTPUT_VARIABLE ${output_var}
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    if ("${${output_var}}" STREQUAL "")
        message(FATAL_ERROR "Error: Calling pyside_config.py ${option} returned no output.")
    endif()
    if(is_list)
        string (REPLACE " " ";" ${output_var} "${${output_var}}")
    endif()
endmacro()

# Query for the shiboken6-generator path, PySide6 path, Python path, include paths and linker flags.
pyside_config(--shiboken-module-path SHIBOKEN_MODULE_PATH)
pyside_config(--shiboken-generator-path SHIBOKEN_GENERATOR_PATH)
pyside_config(--pyside-path PYSIDE_PATH)

pyside_config(--python-include-path PYTHON_INCLUDE_DIR)
pyside_config(--shiboken-generator-include-path SHIBOKEN_GENERATOR_INCLUDE_DIR 1)
pyside_config(--pyside-include-path PYSIDE_INCLUDE_DIR 1)

pyside_config(--python-link-flags-cmake PYTHON_LINKING_DATA 0)
pyside_config(--shiboken-module-shared-libraries-cmake SHIBOKEN_MODULE_SHARED_LIBRARIES 0)
pyside_config(--pyside-shared-libraries-cmake PYSIDE_SHARED_LIBRARIES 0)

set(SHIBOKEN_PATH "${SHIBOKEN_GENERATOR_PATH}/shiboken6${CMAKE_EXECUTABLE_SUFFIX}")

if(NOT EXISTS ${SHIBOKEN_PATH})
    message(FATAL_ERROR "Shiboken executable not found at path: ${SHIBOKEN_PATH}")
endif()

# Get all relevant Qt include dirs, to pass them on to shiboken.
get_property(QT_WIDGETS_INCLUDE_DIRS TARGET Qt6::Widgets PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
set(INCLUDES "")
foreach(INCLUDE_DIR ${QT_WIDGETS_INCLUDE_DIRS})
    list(APPEND INCLUDES "-I${INCLUDE_DIR}")
endforeach()

# On macOS, check if Qt is a framework build. This affects how include paths should be handled.
get_target_property(QtCore_is_framework Qt6::Core FRAMEWORK)
if (QtCore_is_framework)
    get_target_property(qt_core_library_location Qt6::Core LOCATION)
    # PYSIDE-623: We move up until the directory contains all the frameworks.
    #             This is "lib" in ".../lib/QtCore.framework/Versions/A/QtCore".
    get_filename_component(lib_dir "${qt_core_library_location}/../../../.." ABSOLUTE)
    list(APPEND INCLUDES "--framework-include-paths=${lib_dir}")
endif()

# Set up the options to pass to shiboken.
set(WRAPPED_HEADER ${CMAKE_SOURCE_DIR}/src/pyside/holonpythonwrapper.h)
set(TYPESYSTEM_FILE ${CMAKE_SOURCE_DIR}/src/pyside/holonpackage.xml)

set(SHIBOKEN_OPTIONS --generator-set=shiboken --enable-parent-ctor-heuristic
    --enable-pyside-extensions --enable-return-value-heuristic --use-isnull-as-nb-bool
    --avoid-protected-hack
    ${INCLUDES}
    -I${CMAKE_SOURCE_DIR}
    -T${CMAKE_SOURCE_DIR}
    -T${PYSIDE_PATH}/typesystems
    --output-directory=${CMAKE_CURRENT_BINARY_DIR}
)

# Specify which sources will be generated by shiboken, and their dependencies.
set(GENERATED_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/holon/holon_module_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/holon/holonpythontask_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/holon/holonpythontaskwindow_wrapper.cpp
)

set(GENERATED_SOURCES_DEPENDENCIES
    ${WRAPPED_HEADER}
    ${TYPESYSTEM_FILE}
)

# Add custom target to run shiboken.
add_custom_command(
    OUTPUT ${GENERATED_SOURCES}
    COMMAND ${SHIBOKEN_PATH}
    ${SHIBOKEN_OPTIONS} ${WRAPPED_HEADER} ${TYPESYSTEM_FILE}
    DEPENDS ${GENERATED_SOURCES_DEPENDENCIES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running generator for ${TYPESYSTEM_FILE}."
)

target_sources(${QT_HOLON_LIBRARY}
    PRIVATE ${GENERATED_SOURCES}
)

# We need to include the headers for the module bindings that we use.
set(PYSIDE_ADDITIONAL_INCLUDES "")
foreach(INCLUDE_DIR ${PYSIDE_INCLUDE_DIR})
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtCore")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtGui")
    list(APPEND PYSIDE_ADDITIONAL_INCLUDES "${INCLUDE_DIR}/QtWidgets")
endforeach()

# =============================================================================================
# !!! (The section below is deployment related, so in a real world application you will want to
# take care of this properly with some custom script or tool).
# =============================================================================================
# Enable rpaths so that the example can be executed from the build dir.
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${PYSIDE_PATH} ${SHIBOKEN_MODULE_PATH})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# =============================================================================================
# !!! End of dubious section.
# =============================================================================================

# Apply relevant include and link flags.
target_include_directories(${QT_HOLON_LIBRARY}
    PRIVATE ${PYTHON_INCLUDE_DIR}
    PRIVATE ${SHIBOKEN_GENERATOR_INCLUDE_DIR}
    PRIVATE ${PYSIDE_INCLUDE_DIR}
    PRIVATE ${PYSIDE_ADDITIONAL_INCLUDES}
)

# Find and link to the python library.
list(GET PYTHON_LINKING_DATA 0 PYTHON_LIBDIR)
list(GET PYTHON_LINKING_DATA 1 PYTHON_LIB)
find_library(PYTHON_LINK_FLAGS ${PYTHON_LIB} PATHS ${PYTHON_LIBDIR} HINTS ${PYTHON_LIBDIR})

target_link_libraries(${QT_HOLON_LIBRARY}
    PRIVATE ${PYTHON_LINK_FLAGS}
    PRIVATE ${SHIBOKEN_MODULE_SHARED_LIBRARIES}
    PRIVATE ${PYSIDE_SHARED_LIBRARIES}
)

# Same as CONFIG += no_keywords to avoid syntax errors in object.h due to the usage of the word Slot
target_compile_definitions(${QT_HOLON_LIBRARY}
    PRIVATE QT_NO_KEYWORDS
)

if(WIN32)
    # =============================================================================================
    # !!! (The section below is deployment related, so in a real world application you will want to
    # take care of this properly (this is simply to eliminate errors that users usually encounter.
    # =============================================================================================
    # Circumvent some "#pragma comment(lib)"s in "include/pyconfig.h" which might force to link
    # against a wrong python shared library.

    set(PYTHON_VERSIONS_LIST 3 36 37 38 39)
    set(PYTHON_ADDITIONAL_LINK_FLAGS "")
    foreach(VER ${PYTHON_VERSIONS_LIST})
        set(PYTHON_ADDITIONAL_LINK_FLAGS
            "${PYTHON_ADDITIONAL_LINK_FLAGS} /NODEFAULTLIB:\"python${VER}_d.lib\"")
        set(PYTHON_ADDITIONAL_LINK_FLAGS
            "${PYTHON_ADDITIONAL_LINK_FLAGS} /NODEFAULTLIB:\"python${VER}.lib\"")
    endforeach()

    set_target_properties(${QT_HOLON_LIBRARY} PROPERTIES LINK_FLAGS "${PYTHON_ADDITIONAL_LINK_FLAGS}")

    # Add custom target to hard link PySide6 shared libraries (just like in qmake example), so you
    # don't have to set PATH manually to point to the PySide6 package.
    set(shared_libraries ${SHIBOKEN_MODULE_SHARED_LIBRARIES} ${PYSIDE_SHARED_LIBRARIES})
    foreach(LIBRARY_PATH ${shared_libraries})
        string(REGEX REPLACE ".lib$" ".dll" LIBRARY_PATH ${LIBRARY_PATH})
        get_filename_component(BASE_NAME ${LIBRARY_PATH} NAME)
        file(TO_NATIVE_PATH ${LIBRARY_PATH} SOURCE_PATH)
        file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${BASE_NAME}" DEST_PATH)
        add_custom_command(OUTPUT "${BASE_NAME}"
                            COMMAND mklink /H "${DEST_PATH}" "${SOURCE_PATH}"
                            DEPENDS ${LIBRARY_PATH}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                            COMMENT "Creating hardlink to PySide6 shared library ${BASE_NAME}")

        # Fake target that depends on the previous one, but has special ALL keyword, which means
        # it will always be executed.
        add_custom_target("fake_${BASE_NAME}" ALL DEPENDS ${BASE_NAME})
    endforeach()
    # =============================================================================================
    # !!! End of dubious section.
    # =============================================================================================
endif()
