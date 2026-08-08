cmake_minimum_required(VERSION 3.21)
project(gui_shell VERSION 1.0.0 LANGUAGES CXX)

# ------------------------------------------------------------------------
# Global settings
# ------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(GUI_SHELL_BUILD_TESTS "Build gui_shell unit tests" ON)
option(GUI_SHELL_BUILD_EXAMPLES "Build the gui_shell demo application" ON)
option(GUI_SHELL_INSTALL "Generate install rules for gui_shell" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ------------------------------------------------------------------------
# Dependencies (fetched at configure time)
# ------------------------------------------------------------------------
include(FetchContent)
include(cmake/Dependencies.cmake)

# ------------------------------------------------------------------------
# gui_shell library
# ------------------------------------------------------------------------
add_library(gui_shell STATIC
    src/logger.cpp
    src/settings.cpp
    src/theme.cpp
    src/window.cpp
    src/renderer.cpp
    src/application.cpp
        include/gui_shell/layer_markdown.hpp
        include/gui_shell/demo_layer.hpp
        include/gui_shell/empty_layer.hpp
)
add_library(gui_shell::gui_shell ALIAS gui_shell)

target_include_directories(gui_shell
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(gui_shell
    PUBLIC
        imgui
        glfw
        OpenGL::GL
        spdlog::spdlog
    PRIVATE
        nlohmann_json::nlohmann_json
)

target_compile_features(gui_shell PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(gui_shell PRIVATE /W4 /permissive-)
    target_compile_definitions(gui_shell PUBLIC _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(gui_shell PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ------------------------------------------------------------------------
# Tests / examples
# ------------------------------------------------------------------------
if(GUI_SHELL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(GUI_SHELL_BUILD_EXAMPLES)
    add_subdirectory(examples/demo_app)
endif()

# ------------------------------------------------------------------------
# Install (headers + static libs, for ad-hoc packaging/inspection).
#
# NOTE: The recommended way to consume this module is add_subdirectory()
# or FetchContent (see README) -- both give you the gui_shell::gui_shell
# target directly, with all transitive third-party dependencies wired up
# correctly. A relocatable find_package()-style install is intentionally
# NOT provided, since it would require also re-exporting GLFW/spdlog/ImGui
# (which are vendored via FetchContent, not designed to be installed as
# part of a downstream package) -- add_subdirectory/FetchContent avoids
# that whole class of problems.
# ------------------------------------------------------------------------
if(GUI_SHELL_INSTALL)
    include(GNUInstallDirs)

    install(TARGETS gui_shell
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
