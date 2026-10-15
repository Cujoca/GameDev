cmake_minimum_required(VERSION 3.16)
project(assignment C)

# Builds the game. One file for both CachyOS and macOS; the only
# platform-specific part is the link libraries in the if(APPLE) block below.
#
#   cmake -B out          # configure once
#   cmake --build out     # build
#   ./out/bin/game        # run from the project root, see note below
#
# TileTest and the Examples/ programs are deliberately not targets here --
# they still build with the compile-*.sh scripts in build/.
#
# Configure into out/, not build/: build/ is a source directory in this
# project (it holds TileTest.c and the shell scripts), so letting CMake own
# it would mix generated files in with sources.
#
# Asset paths (TILE_SHEET_PATH in Libs/tiles.h, ASSET_DIR in Core/play.h) are
# relative to the project root, so the binary has to be run from the root.

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Generates out/compile_commands.json so clangd sees the real build flags.
# To switch clangd over to it:
#   ln -sf out/compile_commands.json compile_commands.json && rm compile_flags.txt
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The shell scripts always passed -g, so default to a debug build.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Arch and the brew keg both ship a raylib CMake config, so this resolves on
# either machine without any hints.
find_package(raylib REQUIRED)

# game.c is the only TU that defines RAYGUI_IMPLEMENTATION, so menu.c's
# raygui calls link against the implementation it compiles.
add_executable(game
    Core/game.c
    Core/menu.c
    Core/play.c
    Libs/map.c
    Libs/tiles.c
)

# The root include dir supports "Libs/tiles.h" / "Core/menu.h" /
# "Vendor/raygui.h"; the Libs dir supports the sibling "tiles.h" in map.c.
target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/Libs")
target_compile_options(game PRIVATE -Wall -Wextra)

# The X11 stack on Linux, frameworks on macOS -- the -lrt/-lX11 vs.
# -framework Cocoa split the two shell scripts disagreed on.
if(APPLE)
    target_link_libraries(game PRIVATE raylib
        "-framework Cocoa"
        "-framework IOKit"
        "-framework OpenGL"
        "-framework CoreVideo"
        m
    )
else()
    target_link_libraries(game PRIVATE raylib m pthread dl rt X11)
endif()
