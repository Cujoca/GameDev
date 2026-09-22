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

# Build next to the source file so assets (e.g. apple.png) resolve at runtime
set srcdir (dirname "$code")
set libs (dirname (status --current-filename))/Libs

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

echo "Compiling $code -> $srcdir/$executable"

gcc -std=c99 -Wall -Wextra -g \
    -I"$libs" $rl_cflags \
    "$code" $extra \
    -o "$srcdir/$executable" \
    $rl_libs -lm -lpthread -ldl -lrt -lX11

if test $status -eq 0
    echo "Built $srcdir/$executable"
    echo "Run it with: cd $srcdir; and ./$executable"
else
    echo "Compilation failed."
    exit 1
end
