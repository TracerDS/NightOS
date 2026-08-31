#!/bin/bash

target=$1
if [ -z "$target" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
BUILD_DIR="$SCRIPT_DIR/../archs/$target/build"
RAW_NAME="${target%.*}"

if [ -f "$BUILD_DIR/$target.iso" ]; then
    target="$BUILD_DIR/$target.iso"
fi

./scripts/run.sh $target -s -S