#!/bin/bash

target=$1
if [ -z "$target" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")

if [ -f "$SCRIPT_DIR/../archs/$target/build/$target.iso" ]; then
    target="$SCRIPT_DIR/../archs/$target/build/$target.iso"
fi

./scripts/run.sh $target -s -S