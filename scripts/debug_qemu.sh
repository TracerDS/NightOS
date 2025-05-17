#!/bin/bash

target=$1
if [ -z "$target" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

./scripts/run.sh $target -s -S