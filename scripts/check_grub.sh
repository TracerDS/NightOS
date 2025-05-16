#!/bin/bash

if [[ $1 = "" ]]; then
    echo "Usage: $0 <path to grub file>"
    exit 1
fi

if grub-file --is-x86-multiboot $1; then
    echo "$1 has multiboot"
    exit 0
else
    echo "$1 doesn't have multiboot"
    exit 1
fi