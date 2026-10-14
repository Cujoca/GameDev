#!/bin/fish

read -P "Enter file to compile: " code

read -P "Enter name of executable: " executable

# Bail out early if no source file was given or it does not exist
if test -z "$code"
    echo "No source file given."
    exit 1
end

if not test -f "$code"
    echo "File not found: $code"
    exit 1
end

# Default the executable name to the source file name without its extension
if test -z "$executable"
    set executable (basename "$code" .c)
end

# This script lives in build/, so the project root is one level up. Asset
# paths like TILE_SHEET_PATH and includes like "Libs/tiles.h" are relative
# to the root, so build into build/ and run from the root.
set root (realpath (dirname (status --current-filename))/..)
set libs $root/Libs
set out $root/build/$executable

# raylib on CachyOS/Arch ships a pkg-config file; fall back to plain -lraylib
if pkg-config --exists raylib
    set rl_cflags (pkg-config --cflags raylib | string split -n " ")
    set rl_libs (pkg-config --libs raylib | string split -n " ")
else
    set rl_cflags
    set rl_libs -lraylib
end

# Only pull in the shared sources from Libs/ that this file actually includes
set extra
if test -d "$libs"
    for lib in $libs/*.c
        set header (basename "$lib" .c).h
        if grep -q "include .*$header" "$code"
            set extra $extra "$lib"
        end
    end
end

echo "Compiling $code -> $out"

gcc -std=c99 -Wall -Wextra -g \
    -I"$root" -I"$libs" $rl_cflags \
    "$code" $extra \
    -o "$out" \
    $rl_libs -lm -lpthread -ldl -lrt -lX11

if test $status -eq 0
    echo "Built $out"
    echo "Run it from the project root so assets resolve:"
    echo "  cd $root; and ./build/$executable"
else
    echo "Compilation failed."
    exit 1
end
