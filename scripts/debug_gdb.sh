#!/bin/bash

symbol_file=$1
if [ -z "$symbol_file" ]; then
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
fi

#/opt/cross/bin/i686-elf-
gdb -tui -q \
    -ex 'target remote localhost:1234' \
    -ex 'set disassembly-flavor intel' \
    -ex "set architecture $arch:intel" \
    -ex "symbol-file $symbol_file" \
    -ex 'b __kernel_main__' \
