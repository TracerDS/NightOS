#!/bin/bash

target=$1
if [ -z "$target" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

symbol_target="${target%.*}.sym"

objcopy --only-keep-debug $target $symbol_target
objcopy $target "${target%.*}_debug.${target##*.}"
objcopy --strip-unneeded $target