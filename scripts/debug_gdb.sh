#!/bin/bash

target=$1
if [ -z "$target" ]; then
    echo "Usage: $0 <symbol_file>"
    exit 1
fi

arch=$2

if [ -z "$arch" ]; then
    arch="i386"
else
    if [ "$arch" != "i686" ] && [ "$arch" != "x86_64" ]; then
        echo "Invalid architecture. Use 'i686' or 'x86_64'."
        exit 1
    fi
    if [ "$arch" == "i686" ]; then
        arch="i386"
    fi
    if [ "$arch" == "x86_64" ]; then
        arch="x86-64"
    fi
fi

SCRIPT_DIR=$(dirname "$0")

if [ -f "$SCRIPT_DIR/../archs/$target/build/$target.sym" ]; then
    target="$SCRIPT_DIR/../archs/$target/build/$target.sym"
fi


#/opt/cross/bin/i686-elf-
gdb -tui -q \
    -ex 'target remote localhost:1234' \
    -ex 'set disassembly-flavor intel' \
    -ex "set architecture $arch:intel" \
    -ex "symbol-file $target" \
    -ex 'b __kernel_main__' \
