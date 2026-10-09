#!/usr/bin/env bash
# compile-mac.sh -- macOS build helper for this assignment.
#
# The macOS counterpart to ../compile.sh, which is fish and links -lX11/-lrt
# for Linux. Differences from that script:
#   * takes arguments instead of only prompting
#   * links Cocoa/IOKit/OpenGL frameworks rather than X11
#   * finds raylib through brew, since pkg-config is usually absent on macOS
#   * builds into build/ and runs from the project root, because asset paths
#     like TILE_SHEET_PATH in Libs/tiles.h are relative to the root
#
# Usage:
#   ./build/compile-mac.sh build/TileTest.c            # exe name from source
#   ./build/compile-mac.sh Core/game.c game            # explicit exe name
#   ./build/compile-mac.sh                             # prompts for both
#
# Like ../compile.sh, it links only the Libs/*.c files whose header the
# source actually includes.

set -u

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
root="$(cd "$script_dir/.." && pwd)"
libs="$root/Libs"

# Only fall back to prompting when invoked with no arguments at all, so the
# script stays usable from a Makefile or another script.
interactive=0
if [ $# -eq 0 ]; then
    interactive=1
fi

code="${1:-}"
executable="${2:-}"

if [ -z "$code" ]; then
    printf 'Enter file to compile: '
    read -r code
fi

if [ -z "$code" ]; then
    echo "No source file given." >&2
    exit 1
fi

# Accept a path relative to the cwd or to the project root.
if [ ! -f "$code" ]; then
    if [ -f "$root/$code" ]; then
        code="$root/$code"
    else
        echo "File not found: $code" >&2
        exit 1
    fi
fi
code="$(cd "$(dirname "$code")" && pwd)/$(basename "$code")"

if [ -z "$executable" ] && [ "$interactive" -eq 1 ]; then
    printf 'Enter name of executable (blank for %s): ' "$(basename "$code" .c)"
    read -r executable
fi

# Default the executable name to the source file name without its extension.
if [ -z "$executable" ]; then
    executable="$(basename "$code" .c)"
fi

# raylib: pkg-config if present, otherwise brew, otherwise the usual prefixes.
rl_cflags=""
rl_libs=""

if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists raylib; then
    rl_cflags="$(pkg-config --cflags raylib)"
    rl_libs="$(pkg-config --libs raylib)"
else
    brew_prefix=""
    if command -v brew >/dev/null 2>&1; then
        brew_prefix="$(brew --prefix raylib 2>/dev/null || true)"
    fi

    for prefix in "$brew_prefix" /opt/homebrew /usr/local; do
        if [ -n "$prefix" ] && [ -f "$prefix/include/raylib.h" ]; then
            rl_cflags="-I$prefix/include"
            rl_libs="-L$prefix/lib -lraylib"
            break
        fi
    done

    if [ -z "$rl_libs" ]; then
        echo "Could not find raylib. Install it with: brew install raylib" >&2
        exit 1
    fi
fi

# raylib on macOS needs these rather than X11.
frameworks="-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo"

# Only pull in the shared sources from Libs/ that this file actually includes.
extra=""
if [ -d "$libs" ]; then
    for lib in "$libs"/*.c; do
        [ -f "$lib" ] || continue
        header="$(basename "$lib" .c).h"
        if grep -q "include .*$header" "$code"; then
            extra="$extra $lib"
        fi
    done
fi

out="$root/build/$executable"

echo "Compiling $code -> $out"
if [ -n "$extra" ]; then
    echo "  with Libs:$extra"
fi

# -I"$root" supports "Libs/tiles.h" / "Core/menu.h"; -I"$libs" supports "tiles.h".
if cc -std=c99 -Wall -Wextra -g \
      -I"$root" -I"$libs" $rl_cflags \
      "$code" $extra \
      -o "$out" \
      $rl_libs $frameworks -lm; then
    echo "Built $out"
    echo "Run it from the project root so assets resolve:"
    echo "  cd $root && ./build/$executable"
else
    echo "Compilation failed." >&2
    exit 1
fi
